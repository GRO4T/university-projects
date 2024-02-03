# Opis rozwiązania

## Struktura
* main.cpp - tworzy producentów oraz konsumentów
* buffer.h - zawiera implementację bufora
* monitor.h - biblioteka monitora
* thread_monitor.h - zawiera implementacje monitora odpowiedzialnego za synchronizację wątków

## Implementacja bufora
Bufor jest nieznacznie zmienioną implementacją pobraną ze strony przedmiotu. Do przechowywania danych używa kolejki z biblioteki standardowej. Zawiera wszystkie operacje, które zawierał bufor z poprzedniego zadania.

## Implementacja monitora wątków
ThreadMonitor dziedziczy z klasy Monitor znajdującej się w pliku monitor.h.
Zawiera 4 metody:
* putEven(int nr) - wstawia kolejną liczbę parzystą mod 100 zgodnie z warunkami A1
* getEven(int nr) - wyciąga liczbę parzystą zgodnie z warunkami B1
* putOdd(int nr) - wstawia kolejną liczbę nieparzystą mod 100 zgodnie z warunkami A2
* getOdd(int nr) - wyciąga liczbę nieparzystą zgodnie z warunkami B2

Parametr nr jest numer procesu w obrębie grupy (np. proces A1 nr 5). Służy tylko i wyłącznie do celów demonstracji działania rozwiązania.

#### Struktura metod klasy ThreadMonitor
* wywołaj enter()
* sprawdz czy warunek spełniony, jeśli nie, wywołaj wait() na odpowiednim warunku
* wykonaj swoją pracę
* wywołaj signal() dla odpowiednich warunków jeśli są spełnione warunki ich kontynuacji
* wywołaj leave()