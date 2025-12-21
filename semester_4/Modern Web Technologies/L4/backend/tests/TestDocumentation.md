# Dokumentacja testów API

## Spis treści
- [1. Testy autoryzacji i rejestracji (`authTests.http`)](#1-testy-autoryzacji-i-rejestracji-authtestshttp)
- [2. Testy użytkowników (`userTests.http`)](#2-testy-użytkowników-usertestshttp)
- [3. Testy produktów (`productTests.http`)](#3-testy-produktów-producttestshttp)
- [4. Testy recenzji (`reviewTests.http`)](#4-testy-recenzji-reviewtestshttp)

---

## 1. Testy autoryzacji i rejestracji (`authTests.http`)

**Zakres:**  
Testy rejestracji użytkownika i admina, logowania, dostępu do chronionych endpointów oraz walidacji pól.

**Przypadki testowe:**
- Rejestracja nowego użytkownika (poprawna, brakujące pole, zbyt krótkie hasło, istniejący login, puste body)
- Rejestracja admina (poprawna, bez tokena, z tokenem usera)
- Logowanie
- Dostęp do endpointów wymagających uprawnień (admin/user, bez tokena, z nieprawidłowym tokenem)

**Metody HTTP:**  
POST, GET

**Rezultaty:**  
- Poprawne dane: 201/200 OK, zwracany token lub dane użytkownika
- Błędne dane: 400/401/403, komunikat o błędzie
- Brak uprawnień: 401/403

---

## 2. Testy użytkowników (`userTests.http`)

**Zakres:**  
Testy operacji na użytkownikach: pobieranie listy, usuwanie, edycja, rejestracja.

**Przypadki testowe:**
- Pobranie wszystkich użytkowników (admin, user, bez tokena)
- Usuwanie użytkownika (admin, sam siebie, inny user, bez tokena, nieprawidłowe ID)
- Aktualizacja użytkownika (admin, user, inny user, nieprawidłowe ID, brakujące pola, puste body, zbyt krótkie hasło)
- Rejestracja użytkownika (poprawna, brakujące pole, istniejący login)

**Metody HTTP:**  
GET, DELETE, PUT, POST

**Rezultaty:**  
- Poprawne operacje: 200 OK, zmienione dane lub potwierdzenie usunięcia
- Błędne operacje: 400/401/403/404, komunikat o błędzie

---

## 3. Testy produktów (`productTests.http`)

**Zakres:**  
Testy CRUD produktów, filtrowania, walidacji oraz uprawnień.

**Przypadki testowe:**
- Dodawanie produktu (poprawne, brak tokena, brak nazwy/ceny, cena ujemna, nienumeryczna, zbyt długa nazwa)
- Pobieranie produktów (wszystkie, po ID, po nazwie, nieistniejący produkt)
- Filtrowanie po cenie (domyślnie, rosnąco, malejąco, z minPrice)
- Edycja i usuwanie produktu (właściciel/admin, inny user)
- Aktualizacja z brakującymi polami

**Metody HTTP:**  
POST, GET, PUT, DELETE

**Rezultaty:**  
- Poprawne operacje: 200/201 OK, zwracane dane produktu lub potwierdzenie
- Błędne operacje: 400/401/403/404, komunikat o błędzie

---

## 4. Testy recenzji (`reviewTests.http`)

**Zakres:**  
Testy dodawania, pobierania i usuwania recenzji produktów, walidacji oraz uprawnień.

**Przypadki testowe:**
- Dodanie recenzji (poprawne, bez tokena, brak oceny, brak komentarza, rating spoza zakresu, nienumeryczny rating, nieistniejący produkt)
- Dodanie drugiej recenzji przez tego samego użytkownika (jeśli niedozwolone)
- Pobieranie recenzji
- Usuwanie recenzji (właściciel/admin, inny user, bez tokena, nieprawidłowe ID)

**Metody HTTP:**  
POST, GET, DELETE

**Rezultaty:**  
- Poprawne operacje: 200/201 OK, zwracane recenzje lub potwierdzenie usunięcia
- Błędne operacje: 400/401/403/404, komunikat o błędzie

---

## Uwagi końcowe

- Wszystkie testy pokrywają zarówno przypadki pozytywne (sukces), jak i negatywne (błędy walidacji, brak uprawnień, nieistniejące zasoby).
- Testy wymagają podstawienia odpowiednich tokenów i ID (np. `<USER_TOKEN>`, `<PRODUCT_ID>`).
- Wyniki testów pozwalają zweryfikować poprawność walidacji, autoryzacji oraz obsługi błędów w API.