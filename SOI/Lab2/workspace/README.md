# Opis rozwiązania
Na potrzeby zadania stworzyłem trzy dodatkowe kolejki procesów (ONE_Q, TWO_Q, THREE_Q). Korzystam jednak wciąż z kolejki procesów klasy USER. Wynika to z tego, że próbując natknąłem się na błąd, który polegał na tym, że kolejka ONE_Q pozostawała jednoelementowa przez cały czas wykonania programu. Przy czym nie dochodziło do zakleszczenia po skończeniu pracy przez obecnie wykonynywany proces. A po prostu były one wykonywane jeden po drugim. Po wielu godzinach debugowania stwierdziłem, że bezpieczniej będzie wykorzystać kolejkę USER_Q.

## Szczegóły
Na potrzeby implementacji zdefiniowałem 2 wywołania systemowe setpri oraz getpri pozwalające mi ustawiać oraz odczytywać parametry procesów.
Do wywołania ich z poziomu mikrojądra użyłem modułu MM.
W pliku proc.h zdefiniowałem te parametry oraz inne przydatne makra. Większość zmodyfikowanych elementów programu jest oznaczonych tagiem <modified>.
### Objaśnienie podstawowych parametrów
* act_pri - aktualny priorytet w szeregowaniu ze starzeniem. Deklarowany czas wykonania programu w szeregowaniu SJF.
* prev_act_pri - poprzednia wartość aktualnego priorytetu. Parametr stworzony tylko i wyłącznie w celu prezentacji działania szeregowania ze starzeniem.
* base_pri - priorytet bazowy w szeregowaniu ze starzeniem.
* ptype - typ procesu. Może przyjmować wartości całkowite od 1 do 3. Wartość dla procesu init to 1.

### Implementacja algorytmów
W celu uproszczenia implementacji zdecydowałem się wykonywać większośc pracy w funkcji pick_proc() modułu proc.c.
Funkcje ready() oraz unready() są w zasadzie kopiami obsługi procesów USER_Q tj. wrzucają gotowy proces na początek kolejki. Ich jedyną dodatkową funkcją jest sprawdzenie czy po wykonaniu wszystkich operacji nie pojawił się element w uprzednio pustej kolejce procesów o wyższym priorytecie. Jeśli tak to należy zmienić wskazanie kolejki USER.

#### Procesy klasy 1
Implementacja algorytmu round-robin jest dokładną kopią implementacji na procesów klasy USER.

#### Procesy klasy 2
W funkcji pick_proc najpierw tworzę wskaźniki na strukturę proc. Dwa ponieważ potrzebuję wskazanie na proces poprzedni od tego o obecnie najwyższym priorytecie. Następnie iteruję kolejkę TWO_Q w poszukiwaniu procesu o największym priorytecie. Po jego znalezieniu przepinam go na początek kolejki. Robię to dlatego, że funkcja unready korzysta z założenia proces obecnie wykonywany jest na początku kolejki.

#### Procesy klasy 3
Analogicznie do 2 z tym, że szukamy minimum.

## Testowanie
W katalogu include znajduje się plik scheduling.h zawierający funkcje pomocnicze do wywołań systemowych.
Testy znajdują się w katalogu lab2. Używają one programu test.out, który pozwala na ustawienie parametrów procesu oraz manipulowanie czasem wykonania.
Testy wywoływane są przy użycia komendy make, np. make test1
