# Quatum-Tic-Tac-Toe

## Krótki opis szkieletu aplikacji
Mój szkielet apliakcji opiera sie na dwóch oddzielnie działajcych programach.
Jeden to frontend napisany w JS i w react'cie, udostępniony w formie serwera na localhost:3000.
Druga część tego projektu to backend napisany w C++ w formie API z użyciem biblioteki restbed z którym łączy się frontendowy serwer.

Aktualnie system pozwala:
- stworzyć nową grę o dowolnych wymiarach
- wczytać zawartość gry
- wykonać ruch w grze
- wykrywać cykle
- wykonywać "pomiary"
- rozstrzygać o wyniku gry
- Tryb gry: offline - 2 osobowa

*Użyłem proxy aby ominąć zabezpieczenie CORS czyli łącząc się z serwerem na porcie 3000, dostaniemy również odpowiedź od serwera na porcie 8080.

## Ogólna konfiguracja
Trzeba uruchomić obydwie aplikacje, aby otrzymać działający system.

## Front-End:
### Requirements
- Node.js (installed - latest version)
- npm
- nvm (optional)


### Packages
- react
- react-router-dom
- react-scripts
- anxios
- bootstrap
- sass

*Stworzone przez npm create-react-app \<folder name\>

Przed startem:
- `npm install react-scripts`
- `npm install anxios`
- `npm install bootstrap`
- `npm install sass`


### Konfiguracja
Aplikacja klienta jest skonfigurowana domyślnie na localhost:3000
Jej finalne end pointy to:
- [ GET ] /start => start menu
- [ GET ] /games/{id} => geting game info


### Uruchamianie aplikacji na localhost'cie
Należy uruchomić w folderze client-side-frontend polecenie `npm start`. 


## Back-End:
### Requirements
- restbed [https://github.com/Corvusoft/restbed/tree/master] -> headery dodać do includePath'a g++ oraz zbudowane pliki .so do liblaryPath'a g++'a.
- C++14 or higher

### Konfiguracja 
Serwer jest skonfigurowany na localhost:8080
Jego finalne end pointy to:
- [ POST ] /games/ => creating new game
- [ GET ] /GetGames/{id} => geting game info
- [ POST ] /games/{id}/MakeMove => making move to game with {id}

### Kompilacja i uruchomienie

W folderze server-side-backend należy posłużyc się narzędziem CMake do zbudowania aplikacji oraz następnie ją uruchomić.

### Testownie

Zestaw testów dla serwera znajduję sie w folderze server-side-backend/tests.

Przetestowane są tam wszystkie sytuacje potrzebne do poprawnego działania aplikacji. 

Testy należy zbudować za pomocą CMake z ustawionym odpowiednim targetem.