package main

import (
	"fmt"
	"math/rand"
	"sync/atomic"
	"time"
)

const (
	NrOfProcesses = 15
	MinSteps      = 50
	MaxSteps      = 100
	MinDelay      = 10 * time.Millisecond
	MaxDelay      = 50 * time.Millisecond
)

var (
	StartTime = time.Now()
)

type ProcessState int

const (
	LocalSection ProcessState = iota
	EntryProtocol1
	EntryProtocol2
	EntryProtocol3
	EntryProtocol4
	CriticalSection
	ExitProtocol
)

var (
	ProcessStateImages = []string{
		"LOCAL_SECTION",
		"ENTRY_PROTOCOL1",
		"ENTRY_PROTOCOL2",
		"ENTRY_PROTOCOL3",
		"ENTRY_PROTOCOL4",
		"CRITICAL_SECTION",
		"EXIT_PROTOCOL",
	}
)

const (
	BoardWidth  = NrOfProcesses
	BoardHeight = ExitProtocol + 1
)

type Position struct {
	X, Y int
}

type Trace struct {
	TimeStamp time.Duration
	Id        int
	Position  Position
	Symbol    rune
}

func Print_Traces(traces []Trace) {
	for _, trace := range traces {
		fmt.Printf("%f %d %d %d %c\n",
			float64(trace.TimeStamp)/float64(time.Second),
			trace.Id,
			trace.Position.X,
			trace.Position.Y,
			trace.Symbol,
		)
	}
}

type Max_ticket struct {
	SetChannel chan int32
	GetChannel chan (chan int32)
	ticket     int32
}

type Printer struct {
	traceChannel chan []Trace
	done         chan bool
}

var (
	printer Printer
)

func (p *Printer) Start() {
	p.traceChannel = make(chan []Trace, NrOfProcesses)
	p.done = make(chan bool)

	go func() {
		for i := 0; i < NrOfProcesses; i++ {
			traces := <-p.traceChannel
			Print_Traces(traces)
		}

		fmt.Printf("-1 %d %d %d ",
			NrOfProcesses,
			BoardWidth,
			BoardHeight,
		)
		for _, img := range ProcessStateImages {
			fmt.Printf("%s;", img)
		}

		p.done <- true
	}()
}

var (
	Flags = make([]int32, NrOfProcesses)
)

type Process struct {
	Id             int
	Symbol         rune
	Position       Position
	Steps          int
	Traces         []Trace
	TimeStamp      time.Duration
	localMaxTicket int32
}

func (t *Process) Store_Trace() {
	t.Traces = append(t.Traces, Trace{
		TimeStamp: t.TimeStamp,
		Id:        t.Id,
		Position:  t.Position,
		Symbol:    t.Symbol,
	})
}

func (t *Process) ChangeState(NewState ProcessState) {
	t.Position.Y = int(NewState)
	t.TimeStamp = time.Since(StartTime)
	t.Store_Trace()

	time.Sleep(time.Millisecond) // for printing
}

func (t *Process) Init(id int, symbol rune) {
	t.Id = id
	t.Symbol = symbol

	t.Position = Position{
		X: id,
		Y: int(LocalSection),
	}

	t.Steps = MinSteps + rand.Intn(MaxSteps-MinSteps+1)
	t.TimeStamp = time.Since(StartTime)
	t.Store_Trace()
}

func (t *Process) Start() {
	go func() {
		for i := 0; i < t.Steps/7-1; i++ {
			// LOCAL_SECTION
			time.Sleep(MinDelay + time.Duration(rand.Int63n(int64(MaxDelay-MinDelay))))
			t.ChangeState(LocalSection)

			// ENTRY_PROTOCOL_1
			t.ChangeState(EntryProtocol1)
			atomic.StoreInt32(&Flags[t.Id], 1)
			// Wait for all processes to leave critical section and exit protocol
			for {
				condition := true
				for j := 0; j < NrOfProcesses; j++ {
					if atomic.LoadInt32(&Flags[j]) > 2 {
						condition = false
						break
					}
				}
				if condition {
					break
				}
				time.Sleep(time.Millisecond)
			}

			// ENTRY_PROTOCOL_2
			atomic.StoreInt32(&Flags[t.Id], 3)
			t.ChangeState(EntryProtocol2)

			// Wait for any process in ENTRY_PROTOCOL_1
			for j := 0; j < NrOfProcesses; j++ {
				if atomic.LoadInt32(&Flags[j]) == 1 {
					// ENTRY_PROTOCOL_3
					atomic.StoreInt32(&Flags[t.Id], 2)
					t.ChangeState(EntryProtocol3)

					// Wait for any process in ENTRY_PROTOCOL_4
					for {
						found := false
						for k := 0; k < NrOfProcesses; k++ {
							if atomic.LoadInt32(&Flags[k]) == 4 {
								found = true
								break
							}
						}
						if found {
							break
						}
						time.Sleep(time.Millisecond)
					}
					break
				}
			}

			// ENTRY_PROTOCOL_4
			atomic.StoreInt32(&Flags[t.Id], 4)
			t.ChangeState(EntryProtocol4)

			// Wait for lower id processes to finish exit protocol
			if t.Id != 0 {
				for {
					condition := true
					for j := 0; j < t.Id; j++ {
						if atomic.LoadInt32(&Flags[j]) > 1 {
							condition = false
							break
						}
					}
					if condition {
						break
					}
					time.Sleep(time.Millisecond)
				}
			}

			// CRITICAL_SECTION
			t.ChangeState(CriticalSection)
			time.Sleep(MinDelay + time.Duration(rand.Int63n(int64(MaxDelay-MinDelay))))

			// EXIT_PROTOCOL
			t.ChangeState(ExitProtocol)
			// Wait for higher id processes to leave waiting room
			for {
				condition := true
				for j := t.Id + 1; j < NrOfProcesses; j++ {
					flag := atomic.LoadInt32(&Flags[j])
					if flag == 2 || flag == 3 {
						condition = false
						break
					}
				}

				if condition {
					break
				}
				time.Sleep(time.Millisecond)
			}
			atomic.StoreInt32(&Flags[t.Id], 0)
			t.ChangeState(LocalSection)
		}
		printer.traceChannel <- t.Traces
	}()
}

func main() {
	var Processes [NrOfProcesses]Process

	printer.Start()

	symbol := 'A'
	for i := 0; i < NrOfProcesses; i++ {
		Processes[i].Init(i, symbol)
		symbol++
	}

	for i := 0; i < NrOfProcesses; i++ {
		Processes[i].Start()
	}

	<-printer.done
}
