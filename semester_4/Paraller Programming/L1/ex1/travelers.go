package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfTravelers = 15 // k
	MinSteps      = 10
	MaxSteps      = 100
	MinDelay      = 10 * time.Millisecond // 0.01 sekundy
	MaxDelay      = 50 * time.Millisecond // 0.05 sekundy
	BoardWidth    = 15                    // m
	BoardHeight   = 15                    // n
)

type Traveler struct {
	ID     int
	X, Y   int
	Symbol string
}

func (t *Traveler) Move(steps int, ch chan string, wg *sync.WaitGroup) {
	defer wg.Done()

	for i := 0; i < steps; i++ {
		// Losowe opóźnienie
		delay := MinDelay + time.Duration(rand.Int63n(int64(MaxDelay-MinDelay)))
		time.Sleep(delay)

		// Losowy kierunek ruchu
		direction := rand.Intn(4)
		switch direction {
		case 0: // W górę
			t.Y = (t.Y - 1 + BoardHeight) % BoardHeight
		case 1: // W dół
			t.Y = (t.Y + 1) % BoardHeight
		case 2: // W lewo
			t.X = (t.X - 1 + BoardWidth) % BoardWidth
		case 3: // W prawo
			t.X = (t.X + 1) % BoardWidth
		}

		// Przekazanie informacji o kroku do kanału
		ch <- fmt.Sprintf("%d %d %d %d %s", time.Now().UnixNano(), t.ID, t.X, t.Y, t.Symbol)
	}
}

func Printer(ch chan string, done chan bool) {
	// Print the first required line
	fmt.Printf("-1 %d %d %d\n", NrOfTravelers, BoardWidth, BoardHeight)

	for msg := range ch { // Pętla działa, dopóki kanał jest otwarty
		fmt.Println(msg) // Wypisanie komunikatu
	}
	done <- true // Sygnalizacja zakończenia pracy
}

func main() {
	rand.Seed(time.Now().UnixNano())
	var wg sync.WaitGroup

	// Kanał do komunikacji z Printerem
	ch := make(chan string)
	done := make(chan bool)

	// Uruchomienie Printera jako goroutine
	go Printer(ch, done)

	// Tworzenie podróżników
	for i := 0; i < NrOfTravelers; i++ {
		wg.Add(1) // Fixed: Add 1 for each traveler

		traveler := Traveler{
			ID:     i,
			X:      rand.Intn(BoardWidth),
			Y:      rand.Intn(BoardHeight),
			Symbol: fmt.Sprintf("%c", 'A'+(i%26)),
		}
		steps := MinSteps + rand.Intn(MaxSteps-MinSteps+1)
		go traveler.Move(steps, ch, &wg) // Uruchomienie podróżnika jako goroutine
	}

	// Oczekiwanie na zakończenie wszystkich podróżników
	wg.Wait() // Wait for all travelers to finish
	close(ch) // Zamknięcie kanału
	<-done    // Oczekiwanie na zakończenie pracy Printera
}
