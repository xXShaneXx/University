package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfTravelers  = 15 // Liczba podróżników
	MinSteps       = 10
	MaxSteps       = 100
	MinDelay       = 10 * time.Millisecond
	MaxDelay       = 50 * time.Millisecond
	BoardWidth     = 15
	BoardHeight    = 15
	DeadlockTimeot = 2 * MaxDelay
)

type Traveler struct {
	ID     int
	X, Y   int
	Symbol string
}

func (t *Traveler) tryMove(newX int, newY int, board *[BoardWidth][BoardHeight]sync.Mutex) bool {
	locked := make(chan struct{}, 1)

	go func() {
		board[newX][newY].Lock()
		locked <- struct{}{}
	}()

	select {
	case <-locked:
		// Successfully locked the new position
		board[t.X][t.Y].Unlock() // Unlock the current position
		return true
	case <-time.After(DeadlockTimeot):
		// Timeout occurred, failed to lock the new position
		return false
	}
}

func (t *Traveler) Move(steps int, ch chan string, board *[BoardWidth][BoardHeight]sync.Mutex, wg *sync.WaitGroup) {
	defer wg.Done()
	dir := rand.Intn(2)*2 - 1

	for i := 0; i < steps; i++ {
		delay := MinDelay + time.Duration(rand.Int63n(int64(MaxDelay-MinDelay)))
		time.Sleep(delay)

		newX, newY := t.X, t.Y
		switch t.ID % 2 {
		case 0:
			newY = (t.Y + dir + BoardHeight) % BoardHeight
		case 1:
			newX = (t.X + dir + BoardWidth) % BoardWidth
		}

		moveSuccesfull := t.tryMove(newX, newY, board)

		if !moveSuccesfull {
			t.Symbol = string(t.Symbol[0] + 32) // change for small letter
			ch <- fmt.Sprintf("%d %d %d %d %s", time.Now().UnixNano(), t.ID, t.X, t.Y, t.Symbol)
			return
		}
		t.X, t.Y = newX, newY

		ch <- fmt.Sprintf("%d %d %d %d %s", time.Now().UnixNano(), t.ID, t.X, t.Y, t.Symbol)
	}
}

func Printer(ch chan string, done chan bool) {
	fmt.Printf("-1 %d %d %d\n", NrOfTravelers, BoardWidth, BoardHeight)
	for msg := range ch {
		fmt.Println(msg)
	}
	done <- true
}

func main() {
	ch := make(chan string)
	done := make(chan bool)
	var board [BoardWidth][BoardHeight]sync.Mutex
	go Printer(ch, done)

	var wg sync.WaitGroup

	for i := 0; i < NrOfTravelers; i++ {
		wg.Add(1)

		traveler := Traveler{
			ID:     i,
			X:      i,
			Y:      i,
			Symbol: fmt.Sprintf("%c", 'A'+(i%26)),
		}
		board[traveler.X][traveler.Y].Lock()

		steps := MinSteps + rand.Intn(MaxSteps-MinSteps+1)
		go traveler.Move(steps, ch, &board, &wg)
	}

	wg.Wait() // Czekanie na zakończenie wszystkich podróżników
	close(ch)
	<-done
}
