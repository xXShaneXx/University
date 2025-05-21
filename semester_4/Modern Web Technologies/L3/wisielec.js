class Board {
    constructor() {
        this.numSequance = 0;
        this.drawSteps = [];
    }

    init() {
        this.canvas = document.getElementById("canvas");
        this.ctx = this.canvas.getContext("2d");
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.drawSteps = [
            () => { this.line(50, 180, 150, 180); },
            () => { this.line(100, 180, 100, 30); },
            () => { this.line(100, 30, 150, 30); },
            () => { this.line(150, 30, 150, 50); },
            () => { this.circle(150, 60, 10); },
            () => { this.line(150, 70, 150, 110); },
            () => { this.line(150, 80, 160, 100); },
            () => { this.line(150, 80, 140, 100); },
            () => { this.line(150, 110, 140, 140); },
            () => { this.line(150, 110, 160, 140); }
        ];
    }

    line(x1, y1, x2, y2) {
        this.ctx.beginPath();
        this.ctx.moveTo(x1, y1);
        this.ctx.lineTo(x2, y2);
        this.ctx.stroke();
    }

    circle(x, y, r) {
        this.ctx.beginPath();
        this.ctx.arc(x, y, r, 0, Math.PI * 2);
        this.ctx.stroke();
    }

    draw() {
        if (this.numSequance < this.drawSteps.length) {
            this.drawSteps[this.numSequance++]();
        }
    }

    reset() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.numSequance = 0;
    }
}

class Game {
    constructor() {
        this.words = ["konstantynopolitańczykowianeczka", "banan", "kot", "duch", "pies"];
        this.maxTrials = 10;
        this.resetGameVars();
        this.board = new Board();
        this.isGameActive = true;
    }

    init() {
        this.board.init();
        this.loadGameState();
        if (!this.currentWord) { // Jeśli nie ma zapisanej gry
            this.pickWord();
        }
        this.createWordDisplay();
        this.createLetterButtons("aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż");
        this.restoreUI();
        this.initResetButtons();
        this.isGameActive = true;
    }

    pickWord() {
        const index = Math.floor(Math.random() * this.words.length);
        this.currentWord = this.words[index];
    }

    createWordDisplay() {
        const container = document.getElementById("word-container");
        container.innerHTML = "";
        for (const char of this.currentWord) {
            const span = document.createElement("span");
            span.textContent = " "; 
            span.dataset.letter = char;
            container.appendChild(span);
        }
    }

    createLetterButtons(letters) {
        const con = document.getElementById("letters-con");
        con.innerHTML = "";
        for (const letter of letters) {
            const btn = document.createElement("button");
            btn.textContent = letter;
            btn.addEventListener("click", e => this.handleGuess(e));
            con.appendChild(btn);
        }
    }

    handleGuess(event) {
        if (!this.isGameActive) return;
        const button = event.target;

        if (button.disabled) return;
        button.disabled = true;
        button.style.backgroundColor = "blue";

        const guess = button.textContent;
        if (this.currentWord.includes(guess)) {
            this.revealLetters(guess);
            this.checkWin();
        } else {
            this.remainingTrials--;
            this.board.draw();
            this.checkLose();
        }

        this.saveGameState(); 
    }

    revealLetters(letter) {
        const spans = document.getElementById("word-container").children;
        for (const span of spans) {
            if (span.dataset.letter === letter) {
                if (span.textContent === " ") {
                    span.textContent = letter;
                    this.correctGuesses++;
                }
            }
        }
    }

    async checkWin() {
        if (this.correctGuesses === this.currentWord.length) {
            this.isGameActive = false;
            await Swal.fire({
                title: 'Gratulacje!',
                text: 'Udało Ci się odgadnąć słowo!',
                icon: 'success',
                confirmButtonText: 'Nowa gra'
            });
            this.resetGame();
        }
    }

    async checkLose() {
        if (this.remainingTrials <= 0) {
            this.isGameActive = false;
            const result = await Swal.fire({
                title: 'Przegrana!',
                text: `Słowo to: ${this.currentWord}`,
                icon: 'error',
                confirmButtonText: 'Nowa gra',
                showCancelButton: true,
                cancelButtonText: 'Zakończ'
            });
            if (result.isConfirmed) {
                this.resetGame();
            }
        }
    }

    resetGameVars() {
        this.correctGuesses = 0;
        this.remainingTrials = this.maxTrials;
        this.currentWord = "";
    }

    resetGame() {
        this.resetGameVars();
        this.board.reset();
        localStorage.removeItem("hangmanGameState"); // <- Usuwamy zapis
        this.init();
    }

    endGame() {
        this.isGameActive = false;
        document.getElementById("letters-con").innerHTML = "";
        localStorage.removeItem("hangmanGameState"); // <- Usuwamy zapis
        Swal.fire('Gra została anulowana.', '', 'info');
    }

    initResetButtons() {
        document.getElementById("reset-btn").addEventListener("click", () => this.resetGame());
        document.getElementById("cancel-btn").addEventListener("click", () => this.endGame());
    }

    saveGameState() {
        const letters = Array.from(document.querySelectorAll("#word-container span")).map(span => span.textContent);
        const buttons = Array.from(document.querySelectorAll("#letters-con button")).map(btn => ({
            letter: btn.textContent,
            disabled: btn.disabled
        }));

        const state = {
            currentWord: this.currentWord,
            letters,
            buttons,
            remainingTrials: this.remainingTrials,
            numSequance: this.board.numSequance,
            isGameActive: this.isGameActive
        };

        localStorage.setItem("hangmanGameState", JSON.stringify(state));
    }

    loadGameState() {
        const saved = localStorage.getItem("hangmanGameState");
        if (saved) {
            const state = JSON.parse(saved);
            this.currentWord = state.currentWord;
            this.remainingTrials = state.remainingTrials;
            this.board.numSequance = state.numSequance;
            this.isGameActive = state.isGameActive;
            this.savedLetters = state.letters;
            this.savedButtons = state.buttons;
        }
    }

    restoreUI() {
        // Przywróć narysowane elementy
        for (let i = 0; i < this.board.numSequance; i++) {
            this.board.drawSteps[i]();
        }

        // Przywróć litery
        const spans = document.getElementById("word-container").children;
        if (this.savedLetters) {
            for (let i = 0; i < spans.length; i++) {
                spans[i].textContent = this.savedLetters[i];
                if (this.savedLetters[i] !== " ") {
                    this.correctGuesses++;
                }
            }
        }

        // Przywróć przyciski
        if (this.savedButtons) {
            const buttons = document.querySelectorAll("#letters-con button");
            for (const savedButton of this.savedButtons) {
                for (const btn of buttons) {
                    if (btn.textContent === savedButton.letter) {
                        btn.disabled = savedButton.disabled;
                        if (savedButton.disabled) {
                            btn.style.backgroundColor = "blue";
                        }
                    }
                }
            }
        }
    }
}

const game = new Game();
window.onload = () => game.init();

