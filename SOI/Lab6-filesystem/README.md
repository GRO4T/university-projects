# Opis rozwiązania

## Struktura
* main.cpp - obsługa argumentów podanych do programu i wywoływanie odpowiednich funkcji systemu plików
* virtual_filesystem.hpp - plik nagłówkowy klasy systemu plików
* virtual_filesystem.cpp - plik źródłowy zawierający definicje metod klasy
* filesystem_exception.hpp - plik nagłówkowy zawierający definicje wyjątków
* katalog files - zawiera pliki przydatne przy testowaniu systemu plików
* test.sh - testy

## System plików
System plików jest typu Unix'owego. Używa struktury INode do przechowywania informacji o rozmiarze i położeniu pliku na dysku. Dysk jest podzielony na bloki, z czego każdy ma po 1024 bajty. Część dysku jest zarezerwowana na bloki systemowe, w których przechowywane są INode'y.

### API
* konstruktor - jedyne co robi to ustawia nazwę pliku zawierającego wirtualny system plików
* destruktor - zapisuje bloki systemowe do pliku
* create - tworzy plik wirtualnego systemu plików o zadanym rozmiarze
* open - zczytuje bloki systemowe do pamięci operacyjnej
* uploadFile - kopiuje plik o danej nazwie na dysk systemu plików (opcjonalnie można podać nazwę docelową)
* downloadFile - kopiuje plik o danej nazwie z dysku systemu plików (opcjonalnie można podać nazwę docelową)
* renameFile - zmienia nazwę pliku
* removeFile - usuwa plik z systemu plików
* list - wyświetla informacje o plikach
* displayFilemap - wyświetla zajętość dysku w blokach
* displayDetailedFilemap - wyświetla zajętość dysku w kawałkach o wielkości 64 bajtów

### Funkcje pomocnicze
* findINode - szuka INode o podanej nazwie. Jeśli go znajdzie to zwraca wskazanie w przeciwnym razie zwraca NULL
* findINode - analogicznie tylko zwraca indeks INode'a lub -1 jeśli nie znaleziono.
* alloc - zwraca indeks pierwszego bloku, w którym moglibyśmy umieścić nasz plik
* defragment - przemieszcza bloki tak, aby zniwelować fragmentację zewnętrzną
* close - wywoływana przez destruktor