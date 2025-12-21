with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Ada.Wide_Characters.Handling;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Characters.Handling; use Ada.Characters.Handling;

procedure Travelers is

  -- All parameters set to 15 as required
  Nr_Of_Travelers : constant Integer := 15;
  Board_Width     : constant Integer := 15;
  Board_Height    : constant Integer := 15;

  Min_Steps : constant Integer := 10;
  Max_Steps : constant Integer := 100;

  Min_Delay : constant Duration := 0.01;
  Max_Delay : constant Duration := 0.05;

  -- Timing
  Start_Time : Time := Clock;  -- global starting time

  -- Random seeds for the tasks' random number generators
  Seeds : Seed_Array_Type(1..Nr_Of_Travelers) := Make_Seeds(Nr_Of_Travelers);

  -- Types, procedures and functions
  type Position_Type is record   
    X: Integer range 0 .. Board_Width - 1; 
    Y: Integer range 0 .. Board_Height - 1; 
  end record;     

  -- elementary steps
  procedure Move_Down(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + 1) mod Board_Height;
  end Move_Down;

  procedure Move_Up(Position: in out Position_Type) is
  begin
    Position.Y := (Position.Y + Board_Height - 1) mod Board_Height;
  end Move_Up;

  procedure Move_Right(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + 1) mod Board_Width;
  end Move_Right;

  procedure Move_Left(Position: in out Position_Type) is
  begin
    Position.X := (Position.X + Board_Width - 1) mod Board_Width;
  end Move_Left;

  -- traces of travelers
  type Trace_Type is record         
    Time_Stamp: Duration;       
    Id : Integer;
    Position: Position_Type;      
    Symbol: Character;        
  end record;        

  type Trace_Array_type is array(0 .. Max_Steps) of Trace_Type;

  type Traces_Sequence_Type is record
    Last: Integer := -1;
    Trace_Array: Trace_Array_type;
  end record; 

  procedure Print_Trace(Trace : Trace_Type) is
    Symbol : String := ( ' ', Trace.Symbol );
  begin
    Put_Line(
        Duration'Image(Trace.Time_Stamp) & " " &
        Integer'Image(Trace.Id) & " " &
        Integer'Image(Trace.Position.X) & " " &
        Integer'Image(Trace.Position.Y) & " " &
        (' ', Trace.Symbol) -- print as string to avoid: '
      );
  end Print_Trace;

  procedure Print_Traces(Traces : Traces_Sequence_Type) is
  begin
    for I in 0 .. Traces.Last loop
      Print_Trace(Traces.Trace_Array(I));
    end loop;
  end Print_Traces;

  -- task Printer collects and prints reports of traces
  task Printer is
    entry Report(Traces : Traces_Sequence_Type);
  end Printer;
  
  task body Printer is 
  begin
    for I in 1 .. Nr_Of_Travelers loop
      accept Report(Traces : Traces_Sequence_Type) do
         Print_Traces(Traces);
      end Report;
    end loop;
  end Printer;

  -- travelers
  type Traveler_Type is record
    Id: Integer;
    Symbol: Character;
    Position: Position_Type;
    Direction: Integer; -- 0: up, 1: down, 2: left, 3: right
  end record;

  -- board
  task type Node is
    entry Enter;
    entry Leave;
  end Node;

  task body Node is
  begin
    loop
      select
        accept Enter do
          null;
        end Enter;

        select
          accept Leave do
            null;
          end Leave;
        or
          terminate;
        end select;
      or
        terminate;
      end select;
    end loop;
  end Node;

  -- global board
  Board : array (0 .. Board_Width-1, 0 .. Board_Height-1) of Node;

  -- traveler tasks
  task type Traveler_Task_Type is   
    entry Init(Id: Integer; Seed: Integer; Symbol: Character);
    entry Start;
  end Traveler_Task_Type;  

  task body Traveler_Task_Type is
    G : Generator;
    Traveler : Traveler_Type;
    Time_Stamp : Duration;
    Nr_of_Steps: Integer;
    Traces: Traces_Sequence_Type; 
    NewPosition : Position_Type;

    procedure Store_Trace is
    begin  
      Traces.Last := Traces.Last + 1;
      Traces.Trace_Array(Traces.Last) := ( 
          Time_Stamp => Time_Stamp,
          Id => Traveler.Id,
          Position => Traveler.Position,
          Symbol => Traveler.Symbol
        );
    end Store_Trace;
    
    procedure Make_Step is
    begin
      case Traveler.Direction is
        when 0 => Move_Up(NewPosition);
        when 1 => Move_Down(NewPosition);
        when 2 => Move_Left(NewPosition);
        when 3 => Move_Right(NewPosition);
        when others => null;
      end case;
    end Make_Step;

  begin
    accept Init(Id: Integer; Seed: Integer; Symbol: Character) do
      Reset(G, Seed); 
      Traveler.Id := Id;
      Traveler.Symbol := Symbol;
      
      -- Start at position (i, i) as required
      Traveler.Position := (X => Id, Y => Id);
      
      -- Choose initial direction:
      -- Even IDs move vertically (0 or 1)
      -- Odd IDs move horizontally (2 or 3)
      if Id mod 2 = 0 then
        Traveler.Direction := Integer(Float'Floor(2.0 * Random(G))); -- 0 or 1
      else
        Traveler.Direction := 2 + Integer(Float'Floor(2.0 * Random(G))); -- 2 or 3
      end if;
      
      -- Enter the starting position
      Board(Traveler.Position.X, Traveler.Position.Y).Enter;
      NewPosition := Traveler.Position;
      Store_Trace; -- store starting position
      
      -- Number of steps to be made by the traveler  
      Nr_of_Steps := Min_Steps + Integer(Float(Max_Steps - Min_Steps) * Random(G));
      -- Time_Stamp of initialization
      Time_Stamp := To_Duration(Clock - Start_Time);
    end Init;
    
    -- wait for initialisations of the remaining tasks:
    accept Start do
      null;
    end Start;

    for Step in 0 .. Nr_of_Steps loop
      delay Min_Delay + (Max_Delay - Min_Delay) * Duration(Random(G));
      Make_Step;
      select
        Board(NewPosition.X, NewPosition.Y).Enter;
        Board(Traveler.Position.X, Traveler.Position.Y).Leave;
        Traveler.Position := NewPosition;
      or
        delay 3*Max_Delay;
        Traveler.Symbol := To_Lower(Traveler.Symbol);
        Store_Trace;
        exit;
      end select;
      Store_Trace;
      Time_Stamp := To_Duration(Clock - Start_Time);
    end loop;
    Printer.Report(Traces);
  end Traveler_Task_Type;

  -- local for main task
  Travel_Tasks: array (0 .. Nr_Of_Travelers-1) of Traveler_Task_Type;
  Symbol : Character := 'A';
begin 
  
  -- Print the line with the parameters needed for display script:
  Put_Line(
      "-1 "&
      Integer'Image(Nr_Of_Travelers) &" "&
      Integer'Image(Board_Width) &" "&
      Integer'Image(Board_Height)      
    );

  -- init travelers tasks
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Init(I, Seeds(I+1), Symbol);
    Symbol := Character'Succ(Symbol);
  end loop;

  -- start travelers tasks
  for I in Travel_Tasks'Range loop
    Travel_Tasks(I).Start;
  end loop;
end Travelers;