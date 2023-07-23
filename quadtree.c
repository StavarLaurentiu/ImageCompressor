#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pixel {
    unsigned char red, green, blue;
} Pixel;

typedef struct Node {
    unsigned char type;
    struct Pixel *rgb_content;
    struct Node *left_top, *right_top, *left_bottom, *right_bottom;
} Node;

typedef struct Queue {
    Node *node;
    struct Queue *next;
} Queue;

int my_max(int a, int b, int c, int d);
int my_min(int a, int b, int c, int d);
void enqueue(Queue **rear, Node *node);
Node *init_tree(Node *root);
void create_compression_tree(Node *root, Pixel **image, int x, int y,
                             unsigned int height, unsigned int width,
                             int factor, int *num_end_node);
void print_tree(Node *root);
int depth(Node *root);
int depth_first_end_node(Node *root);
unsigned long long compute_mean(Pixel **image, unsigned long long *red,
                                unsigned long long *green,
                                unsigned long long *blue, int x, int y,
                                unsigned int height, unsigned int width);
Pixel **read_image(unsigned int *height, unsigned int *width, FILE *fid);
void write_compression_tree(Node *root, FILE *fid);
void read_compression_tree(Node *root, FILE *fid);
void print_image(Pixel **image, unsigned int height, unsigned int width,
                 FILE *fid);
void free_image(Pixel **image, unsigned int height);
void free_compression_tree(Node *root);               

int my_max(int a, int b, int c, int d) {
    int max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    if (d > max) {
        max = d;
    }

    return max;
}

int my_min(int a, int b, int c, int d) {
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    if (d < min) {
        min = d;
    }

    return min;
}

// Add a node to the queue
void enqueue(Queue **rear, Node *node) {
    if (node == NULL) {
        return;
    }

    Queue *new_node = malloc(sizeof(Queue));
    new_node->node = node;
    new_node->next = NULL;

    if ((*rear) == NULL) {
        (*rear) = new_node;
        return;
    }

    (*rear)->next = new_node;
    (*rear) = new_node;
}

// Initialize the tree
Node *init_tree(Node *root) {
    root->type = 0;
    root->rgb_content = NULL;
    root->left_top = NULL;
    root->right_top = NULL;
    root->left_bottom = NULL;
    root->right_bottom = NULL;

    return root;
}

// Compute the depth of the tree recursively
int depth(Node *root) {
    if (root == NULL) {
        return 0;
    }

    return my_max(1 + depth(root->left_top), 1 + depth(root->right_top),
                  1 + depth(root->left_bottom), 1 + depth(root->right_bottom));
}

// Compute the depth of the closest end node to the root recursively
int depth_first_end_node(Node *root) {
    if (root == NULL) {
        return 0;
    }

    return my_min(1 + depth_first_end_node(root->left_top),
                  1 + depth_first_end_node(root->right_top),
                  1 + depth_first_end_node(root->left_bottom),
                  1 + depth_first_end_node(root->right_bottom));
}

// Compute the value of mean with the formula
unsigned long long compute_mean(Pixel **image, unsigned long long *red,
                                unsigned long long *green,
                                unsigned long long *blue, int x, int y,
                                unsigned int height, unsigned int width) {
    // Compute the read, green and blue values
    int i, j;
    for (i = x; i < x + height; i++) {
        for (j = y; j < y + width; j++) {
            (*red) += image[i][j].red;
            (*green) += image[i][j].green;
            (*blue) += image[i][j].blue;
        }
    }

    (*red) /= (height * width);
    (*green) /= (height * width);
    (*blue) /= (height * width);

    (*red) = floor((*red));
    (*green) = floor((*green));
    (*blue) = floor((*blue));

    // Compute the mean
    unsigned long long mean = 0;
    for (i = x; i < x + height; i++) {
        for (j = y; j < y + width; j++) {
            mean += ((*red) - image[i][j].red) * ((*red) - image[i][j].red) +
                    ((*green) - image[i][j].green) *
                        ((*green) - image[i][j].green) +
                    ((*blue) - image[i][j].blue) * ((*blue) - image[i][j].blue);
        }
    }

    return floor(mean / (3 * height * width));
}

// Create the compression tree using DFS algorithm
void create_compression_tree(Node *root, Pixel **image, int x, int y,
                             unsigned int height, unsigned int width,
                             int factor, int *num_end_node) {
    unsigned long long red = 0, green = 0, blue = 0;
    unsigned long long mean =
        compute_mean(image, &red, &green, &blue, x, y, height, width);

    //  The node is an end node or we continue to divide the image
    if (mean <= factor) {
        root->type = 1;
        root->rgb_content = malloc(sizeof(Pixel));
        root->rgb_content->red = red;
        root->rgb_content->green = green;
        root->rgb_content->blue = blue;
        (*num_end_node)++;
    } else {
        // Allocate memory for the 4 quarters of the image
        root->type = 0;
        root->rgb_content = NULL;
        root->left_top = malloc(sizeof(Node));
        root->right_top = malloc(sizeof(Node));
        root->right_bottom = malloc(sizeof(Node));
        root->left_bottom = malloc(sizeof(Node));
        root->left_top = init_tree(root->left_top);
        root->right_top = init_tree(root->right_top);
        root->right_bottom = init_tree(root->right_bottom);
        root->left_bottom = init_tree(root->left_bottom);

        // Recursively call the function for each quarter of the image
        create_compression_tree(root->left_top, image, x, y, height / 2, width / 2, factor,
                    num_end_node);
        create_compression_tree(root->right_top, image, x, y + width / 2, height / 2,
                    width / 2, factor, num_end_node);
        create_compression_tree(root->right_bottom, image, x + height / 2, y + width / 2,
                    height / 2, width / 2, factor, num_end_node);
        create_compression_tree(root->left_bottom, image, x + height / 2, y, height / 2,
                    width / 2, factor, num_end_node);
    }
}

// Read the image from the file
Pixel **read_image(unsigned int *height, unsigned int *width, FILE *fid) {
    // Read the header
    fseek(fid, 3, SEEK_CUR);
    fscanf(fid, "%u%u", height, width);
    fseek(fid, 5, SEEK_CUR);

    // Allocate memory for the image
    Pixel **image = malloc(*height * sizeof(Pixel *));
    int i, j;
    for (i = 0; i < *height; i++) {
        image[i] = malloc(*width * sizeof(Pixel));
    }

    // Read the image
    for (i = 0; i < *height; i++) {
        for (j = 0; j < *width; j++) {
            fread(&image[i][j], sizeof(Pixel), 1, fid);
        }
    }

    return image;
}

// Write the image to the file using BFS algorithm
void write_compression_tree(Node *root, FILE *fid) {
    if (root == NULL) {
        return;
    }

    // Order the nodes using a queue
    Queue *front = malloc(sizeof(Queue));
    Queue *rear = front;
    front->node = root;
    front->next = NULL;

    while (front != NULL) {
        Node *current_node = front->node;

        // Write the type of the node
        fwrite(&current_node->type, sizeof(unsigned char), 1, fid);
        if (current_node->type == 0) {
            enqueue(&rear, current_node->left_top);
            enqueue(&rear, current_node->right_top);
            enqueue(&rear, current_node->right_bottom);
            enqueue(&rear, current_node->left_bottom);
        } else {
            // Write the color of the node
            fwrite(current_node->rgb_content, sizeof(Pixel), 1, fid);
        }

        Queue *to_free = front;

        // Dequeue the next node
        front = front->next;

        // Free the memory
        free(to_free);
    }
}

// Read the compressed tree from the file using BFS algorithm
void read_compression_tree(Node *root, FILE *fid) {
    if (root == NULL) {
        return;
    }

    // Order the nodes using a queue
    Queue *front = malloc(sizeof(Queue));
    Queue *rear = front;
    front->node = root;
    front->next = NULL;

    while (front != NULL) {
        Node *current_node = front->node;

        // Read the type of the node
        fread(&current_node->type, sizeof(unsigned char), 1, fid);
        if (current_node->type == 0) {
            current_node->rgb_content = NULL;
            current_node->left_top = malloc(sizeof(Node));
            current_node->right_top = malloc(sizeof(Node));
            current_node->left_bottom = malloc(sizeof(Node));
            current_node->right_bottom = malloc(sizeof(Node));
            current_node->left_top = init_tree(current_node->left_top);
            current_node->right_top = init_tree(current_node->right_top);
            current_node->left_bottom = init_tree(current_node->left_bottom);
            current_node->right_bottom = init_tree(current_node->right_bottom);
            enqueue(&rear, current_node->left_top);
            enqueue(&rear, current_node->right_top);
            enqueue(&rear, current_node->right_bottom);
            enqueue(&rear, current_node->left_bottom);
        } else {
            // Read the color of the node
            current_node->rgb_content = malloc(sizeof(Pixel));
            fread(current_node->rgb_content, sizeof(Pixel), 1, fid);
        }

        Queue *to_free = front;

        // Dequeue the current node
        front = front->next;

        // Free the memory
        free(to_free);
    }
}

// Reconstruct the image from the compressed tree using DFS algorithm
void reconstruct_image(Pixel **image, Node *root, int x, int y,
                       unsigned int height, unsigned int width) {
    if (root == NULL) {
        return;
    }

    if (root->type == 1) {
        // Fill the square with the color of the node
        int i, j;
        for (i = x; i < x + height; i++) {
            for (j = y; j < y + width; j++) {
                image[i][j] = *(root->rgb_content);
            }
        }
    } else {
        // Recursively reconstruct the image
        reconstruct_image(image, root->left_top, x, y, height / 2, width / 2);
        reconstruct_image(image, root->right_top, x, y + width / 2, height / 2,
                          width / 2);
        reconstruct_image(image, root->right_bottom, x + height / 2,
                          y + width / 2, height / 2, width / 2);
        reconstruct_image(image, root->left_bottom, x + height / 2, y,
                          height / 2, width / 2);
    }
}

// Print the image to the file in PPM format
void print_image(Pixel **image, unsigned int height, unsigned int width,
                 FILE *fid) {
    // Print the header
    fprintf(fid, "P6\n");
    fprintf(fid, "%u %u\n", width, height);
    fprintf(fid, "255\n");

    // Print the image
    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fwrite(&image[i][j], sizeof(Pixel), 1, fid);
        }
    }
}

void free_image(Pixel **image, unsigned int height) {
    int i;
    for (i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
}

void free_compression_tree(Node *root) {
    if (root == NULL) {
        return;
    }

    free(root->rgb_content);
    if (root->type == 0) {
        free_compression_tree(root->left_top);
        free_compression_tree(root->right_top);
        free_compression_tree(root->right_bottom);
        free_compression_tree(root->left_bottom);
    }
    
    free(root);
}

int main(int argc, char **argv) {
    // Check if the arguments are correct
    if ((argc != 4 && argc != 5) ||
        (strcmp(argv[1], "-c1") == 0 && argc != 5) ||
        (strcmp(argv[1], "-c2") == 0 && argc != 5) ||
        (strcmp(argv[1], "-d") == 0 && argc != 4)) {
        printf(
            "Usage: ./quadtree [-c1 factor | -c2 factor | -d] [input_file] "
            "[output_file]\n");
        exit(0);
    }

    // Input and output files
    FILE *fid_in = NULL;
    FILE *fid_out = NULL;

    // The image and dimensions
    Pixel **image = NULL;
    unsigned int width = 0, height = 0;

    // The root of the compression tree
    Node *root = malloc(sizeof(Node));
    root = init_tree(root);

    // Check which command was given
    if (strcmp(argv[1], "-c1") == 0) {
        // Get the arguments
        int factor = atoi(argv[2]);
        fid_in = fopen(argv[3], "rb");
        fid_out = fopen(argv[4], "wb");

        // Read the PPM file
        image = read_image(&height, &width, fid_in);
        
        // Create the tree
        int x = 0, y = 0, num_end_node = 0;
        create_compression_tree(root, image, x, y, height, width, factor,
                                &num_end_node);

        // Write depth, number of end nodes and length 
        // of the biggest undivided square
        fprintf(fid_out, "%d\n", depth(root));
        fprintf(fid_out, "%d\n", num_end_node);
        fprintf(fid_out, "%d\n",
                (int)(height / pow(2, depth_first_end_node(root) - 1)));
    } else if (strcmp(argv[1], "-c2") == 0) {
        // Get the arguments
        int factor = atoi(argv[2]);
        fid_in = fopen(argv[3], "rb");
        fid_out = fopen(argv[4], "wb");

        // Read the PPM file
        image = read_image(&height, &width, fid_in);

        // Create the tree
        int x = 0, y = 0, num_end_node = 0;
        create_compression_tree(root, image, x, y, height, width, factor,
                                &num_end_node);

        // Write the compressed file
        fwrite(&height, sizeof(unsigned int), 1, fid_out);
        write_compression_tree(root, fid_out);
    } else if (strcmp(argv[1], "-d") == 0) {
        // Get the arguments
        fid_in = fopen(argv[2], "rb");
        fid_out = fopen(argv[3], "wb");

        // Read the size of the image
        fread(&height, sizeof(unsigned int), 1, fid_in);
        width = height;

        // Read the tree
        read_compression_tree(root, fid_in);

        // Allocate memory for the image
        image = malloc(height * sizeof(Pixel *));
        int i;
        for (i = 0; i < height; i++) {
            image[i] = malloc(width * sizeof(Pixel));
        }

        // Reconstruct the image
        int x = 0, y = 0;
        reconstruct_image(image, root, x, y, height, width);

        // Print the image
        print_image(image, height, width, fid_out);
    } else {
        printf("Invalid command\n");
        printf(
            "Usage: ./quadtree [-c1 factor | -c2 factor | -d] [fisier_intrare] "
            "[fisier_iesire]\n");
    }

    // Close the files
    fclose(fid_in);
    fclose(fid_out);

    // Free the memory
    free_image(image, height);
    free_compression_tree(root);

    return 0;
}
