# Opis rozwiązania
## Struktura projektu
* main.c - tworzy oraz inicjuje niezbędne obiekty
* buffer.h - plik nagłówkowy zawierający implementację bufora
* proc_factory - plik nagłówkowy zawierający fabrykę procesów

### Implementacja bufora
Bufor jest zaimplementowany jako tablica liczb całkowitych. Przechowuje on wartości indeksów przodu oraz tyłu kolejki.
Do synchronizacji używam semafora z pliku nagłówkowego **<semaphore.h>**.

#### Funkcje bufora
* Buffer__bind() - inicjuje dzielony obszar pamięci bufora. Zwraca zainicjowany wskaźnik na bufor.
* Buffer__init() - inicjuje semafory oraz liczniki.
* Buffer__put(value) - umieszcza wartość **value** na końcu kolejki. Aktualizuje liczniki.
* Buffer__pop() - zdejmuje oraz zwraca wartość z czoła kolejki. Aktualizuje liczniki.
* Buffer__peek() - pozwala bez zdejmowania podejrzeć wartość na czele kolejki. 
* Buffer__get_size() - zwraca ilość elementów w kolejce
* Buffer__get_num_even() - zwraca ilość parzystych elementów
* Buffer__get_num_odd() - zwraca ilość elementów nieparzystych
* Buffer__print() - wypisuje zawartość bufora na wyjście

### Fabryka procesów
W pliku **proc_factory.h** znajduje się struktura umożliwiająca tworzenie procesów czterech typów:
* A1 - producent umieszczający liczby parzyste mod 100, jeśli w kolejce jest mniej niż 10 elementów
* A2 - producent umieszczający liczby nieparzyste mod 100, jeśli w kolejce jest wiecej liczb parzystych niż nieparzystych
* B1 - konsument usuwający parzyste wartości z czoła kolejki, jeśli w kolejce są co najmniej 3 elementy
* B2 - konsument usuwający nieparzyste wartości z czoła kolejki jeśli w kolejce 
jest co najmniej 7 elementów

#### Synchronizacja
Do synchronizacji procesów używam łącznie 8 semaforów.
Cztery semafory condition odpowiadają za warunki związanie z liczbą elementów w kolejce.
Semafory even oraz odd odpowiadają za parzystość elementów.
Semafory a1_val_lock oraz a2_val_lock są zamkami zapewniającymi synchronizację przy generacji kolejnych elementów przez procesy A1 oraz A2.

Każdy proces przed wejściem do bufora napotyka pętlę, w której sprawdza czy warunek wejścia jest spełniony. Jeśli nie, wywołuje operację wait na odpowiednim semaforze. Pętla zapewnia, że semafory będą poprawnie zerowane.

Po wyjściu z bufora proces sprawdza warunki kontynuacji siebie oraz innych procesów. Jeśli są spełniony wywołuje metodę post na odpowiednim semaforze. Taka operacja obudzi proces, który czekał na odpowiedni warunek kontynuacji. Jeśli żaden proces nie czekał na semaforze, to wcześniej wspomniane pętle zapewnią poprawne wyzerowanie semafora.

#### Funkcje fabryki procesów
* _Proc_factory__get_a1_next_val() - zwraca kolejną liczbę parzystą mod 100
* _Proc_factory__get_a2_next_val() - zwraca kolejną liczbę nieparzystą mod 100
* Proc_factory__bin() - inicjuje dzielony obszar pamięci dla fabryki. Zwraca wskaźnik na fabrykę
* Proc_factory__bind() - inicjuje semafory oraz liczniki wartości
* _Proc_factory__XX_job(buf) - wykonuje pętle procesu, gdzie XX oznacza typ procesu
* Proc_factory__spawn_proc(buf, p_type) - tworzy nowy proces i w zależności od podanej wartości p_type wykonuje odpowiednią procedurę.