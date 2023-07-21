///////////////////////////////////////////////////////////////////////////////

Tema 2 - Structuri de Date si Algoritmi - Stavar Laurentiu - 312CC

• Punctaj checker local: - Teste: 80p;
                         - Valgrind: 20p;

• Durata implementare: ~16h;

• Descriere:

Acest program este o implementare in C a compresiei de imaginii folosid un 
arbore cuaternar;

Programul preia un fisier de tip imagine necomprimat in format PPM si il 
comprima folosind un arbore cuaternar. Imaginea comprimata este salvata 
intr-un fisier binar, împreuna cu arborele compresie folosit. De asemenea, 
programul poate decomprima imaginea comprimata si recrea imaginea initiala
necomprimata;

• Folosire:

Programul primeste trei argumente de intrare:
    - comanda (c1, c2 sau d);
    - numele fisierului de intrare;
    - numele fisierului de iesire;

    Comanda c1 va comprima imaginea folosind un arbore cuaternar si va scrie
intr-un fisier text numarul de niveluri din arborele de compresie, numarul
de noduri din arborele de compresie si latura celui mai mare patrat ramas
necomprimat;
    Comanda c2 va comprima imaginea folosind un arbore cuaternar si va salva
arborele de compresie intr-un fisier binar;
    Comanda d va decomprima imaginea folosind arborele de compresie si va 
salva imaginea decomprimata intr-un fisier de tip PPM;

• Compilare:
  -> make
  -> make clean

• Rulare:
  ./quadtree -c1 in.ppm out.txt
  ./quadtree -c2 in.ppm out.txt
  ./quadtree -d in.ppm out.ppm

• Mesaj de eroare:
  Usage ./quadtree [-c1 factor | -c2 factor | -d] [input_file] [output_file]

• Structuri de date folosite:
  -> Arbore cuaternar;
  -> Pixel (contine culoarea unui pixel prin 3 valori intregi);
  -> Coada implementata cu lista simplu inlantuita;
  -> Lista simplu inlantuita;
  -> Matrice;
  -> Vectori;

///////////////////////////////////////////////////////////////////////////////
