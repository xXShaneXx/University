package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfTravelers  = 15
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

	for i := 0; i < steps; i++ {
		delay := MinDelay + time.Duration(rand.Int63n(int64(MaxDelay-MinDelay)))
		time.Sleep(delay)

		direction := rand.Intn(4)
		newX, newY := t.X, t.Y
		switch direction {
		case 0:
			newY = (t.Y - 1 + BoardHeight) % BoardHeight
		case 1:
			newY = (t.Y + 1) % BoardHeight
		case 2:
			newX = (t.X - 1 + BoardWidth) % BoardWidth
		case 3:
			newX = (t.X + 1) % BoardWidth
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

func generateShuffledPositions() [][2]int {
	positions := make([][2]int, 0, BoardWidth*BoardHeight)
	for x := 0; x < BoardWidth; x++ {
		for y := 0; y < BoardHeight; y++ {
			positions = append(positions, [2]int{x, y})
		}
	}
	rand.Shuffle(len(positions), func(i, j int) {
		positions[i], positions[j] = positions[j], positions[i]
	})
	return positions
}

func main() {
	ch := make(chan string)
	done := make(chan bool)
	var board [BoardWidth][BoardHeight]sync.Mutex
	go Printer(ch, done)

	var wg sync.WaitGroup

	positions := generateShuffledPositions()

	for i := 0; i < NrOfTravelers; i++ {
		wg.Add(1)

		pos := positions[i]
		traveler := Traveler{
			ID:     i,
			X:      pos[0],
			Y:      pos[1],
			Symbol: fmt.Sprintf("%c", 'A'+(i%26)),
		}
		board[traveler.X][traveler.Y].Lock()

		steps := MinSteps + rand.Intn(MaxSteps-MinSteps+1)
		go traveler.Move(steps, ch, &board, &wg)
	}

	wg.Wait() // Wait for all travelers
	close(ch)
	<-done
}
