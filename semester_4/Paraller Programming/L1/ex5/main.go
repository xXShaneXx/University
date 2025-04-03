package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfTravelers = 15 // Liczba podróżników
	MinSteps      = 10
	MaxSteps      = 100
	MinDelay      = 10 * time.Millisecond
	MaxDelay      = 50 * time.Millisecond
	BoardWidth    = 15
	BoardHeight   = 15
)

type Board struct {
	mu    sync.Mutex
	cells [BoardHeight][BoardWidth]bool
}

func (b *Board) takePlace(x, y int) bool {
	b.mu.Lock()
	defer b.mu.Unlock()
	if !b.cells[y][x] {
		b.cells[y][x] = true
		return true
	}
	return false
}

func (b *Board) leavePlace(x, y int) {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.cells[y][x] = false
}

type Traveler struct {
	ID     int
	X, Y   int
	Symbol string
}

func (t *Traveler) Move(steps int, ch chan string, board *Board, wg *sync.WaitGroup) {
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

		if board.takePlace(newX, newY) {
			board.leavePlace(t.X, t.Y)
			t.X, t.Y = newX, newY
		} else {
			// Jeśli nie uda się przemieścić, podróżnik się "poddaje"
			t.Symbol = string(t.Symbol[0] + 32) // Zamiana symbolu na małą literę
			ch <- fmt.Sprintf("DEADLOCK %d %d %d %s", t.ID, t.X, t.Y, t.Symbol)
			return
		}

		ch <- fmt.Sprintf("%d %d %d %d %s", time.Now().UnixNano(), t.ID, t.X, t.Y, t.Symbol)
	}
}

func Printer(ch chan string, done chan bool) {
	for msg := range ch {
		fmt.Println(msg)
	}
	done <- true
}

func main() {
	ch := make(chan string)
	done := make(chan bool)
	board := Board{}
	go Printer(ch, done)

	var wg sync.WaitGroup

	for i := 0; i < NrOfTravelers; i++ {
		wg.Add(1)

		traveler := Traveler{
			ID:     i,
			X:      i,
			Y:      i,
			Symbol: fmt.Sprintf("T%d", i),
		}
		for !board.takePlace(traveler.X, traveler.Y) {
			traveler.X = rand.Intn(BoardWidth)
			traveler.Y = rand.Intn(BoardHeight)
		}
		steps := MinSteps + rand.Intn(MaxSteps-MinSteps+1)
		go traveler.Move(steps, ch, &board, &wg)
	}

	wg.Wait() // Czekanie na zakończenie wszystkich podróżników
	close(ch)
	<-done
}
