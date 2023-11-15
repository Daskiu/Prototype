<script>
  import { each } from "svelte/internal";
  import { onMount, onDestroy } from "svelte/internal";
  import SerialPort from "serialport";

	let rows =[];
	let score = 0;
	let gameover = false;
	let timer;
	let timeLimit = 30000;
	let validKeys = ['q', 'w', 'e', 'r'];

	const backgroundMusic = document.getElementById("backgroundMusic");
  	backgroundMusic.volume = 0.1;

	let serialPort;

	onMount(()=>{
		connectToArduino();
		window.addEventListener("keydown", handleKeyPress);
		startGame();
	});

	onDestroy(() => {
		if (serialPort) {
			serialPort.close();
		}
	});

	function connectToArduino() {
    const portName = "COM3";
    serialPort = new SerialPort(portName, {
      baudRate: 9600,
    });

    serialPort.on("open", () => {
      console.log("Conectado al Arduino");
    });

    serialPort.on("data", (data) => {
      const key = data.toString().trim().toLowerCase();
      if (validKeys.includes(key)) {
        const column = getKeyColumn(key);
        tapped(rows.length - 1, column);
      }
    });
	
	}

	function startGame() {
		fillRows();
  		startTimer();
	}

	function startTimer() {
  		timer = setTimeout(gameOver, timeLimit);
	}

	function gameOver() {
  		gameover = true;
		clearTimeout(timer);
		const backgroundMusic = document.getElementById("backgroundMusic");
 		backgroundMusic.pause();
	}

	function generateRow(){
		let row = new Array(4).fill("white");
		let pos = Math.trunc(Math.random()*4);
		row[pos] = "black";
		return row;
	}

	function fillRows(){
		for(let i=0; i<4; i++){
			rows.push(generateRow());
		}
	}

	function handleKeyPress(event){
		if (!gameover) {
			const key = event.key.toLowerCase();
			if (validKeys.includes(key)){
				const column = getKeyColumn(key);
				tapped(rows.length - 1, column)
			}
		}
	}

	function getKeyColumn(key){
		return validKeys.indexOf(key);
	}

	function tapped(i,j){
		if(i != rows.length - 1 || rows[i][j] == "white"){
			gameover = true;
			gameOver();
			rows[i][j] = "red";
		} else {
			rows.splice(i);
			rows = [generateRow(), ...rows];
			score++;
		}
	}

	function changeVolume(event) {
    	const backgroundMusic = document.getElementById("backgroundMusic");
    	backgroundMusic.volume = parseFloat(event.target.value);
	}

	function restart(){
		score = 0;
		gameover = false;
		rows = [];
		startGame();
		const backgroundMusic = document.getElementById("backgroundMusic");
		backgroundMusic.currentTime = 0;
		backgroundMusic.play();
	}

	startGame();

</script>

<style>
	.app{
		position: fixed;
		top: 0px;
		left: 50%;
		transform: translateX(-50%);
		width: 100%;
		height: 100%;
		max-width: 400px;
	}

	.app .header {
		position: sticky;
		top: 0px;
		left: 0px;
		width: 100%;
		height: 50px;
		background: #0badea;
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 0px 20px;
		color: #fff;
	}

	.app .game{
		width: 100%;
		height: calc(100% - 50px);
		background: #fff;
	}

	.app .game .row {
		display: flex;
		width: 100%;
		height: calc(100% / 4);
	}

	.app .game .row .box {
		flex: 1;
		border: 1px solid #555;
		cursor: pointer;
	}

	.app .game .row .box.black {
		background: #111;
	}

	.app .game .row .box.red {
		animation: blinkRed 500ms ease-in-out infinite;
	}

	@keyframes blinkRed {
		0%, 100% {
			background: #fff;
		}
		50% {
			background: tomato;
		}
	}

	.result {
		position: absolute;
		top: 0px;
		left: 0px;
		width: 100%;
		height: 100%;
		background: rgba(0,0,0,0.8);
		display: flex;
		justify-content: center;
		align-items: center;
		flex-direction: column;
		gap: 10px;
	}

	.result h2 {
		font-size: 40px;
		color: #fff;
	}

	.result p {
		font-size: 20px;
		margin-bottom: 10px;
		color: #eee;
	}

	.result button {
		padding: 10px 20px;
		cursor: pointer;
		font-size: 16px;
		border: 2px solid #fff;
		color: #fff;
		outline: none;
		font-weight: 600;
		background: transparent;
	}

	#volumeControl {
  		width: 100px;
  		height: 5px;
  		-webkit-appearance: none;
  		background: #d3d3d3;
  		outline: none;
  		opacity: 0.7;
  		-webkit-transition: .2s;
  		transition: opacity .2s;
  		margin-top: 10px;
	}

	#volumeControl::-webkit-slider-thumb {
  		-webkit-appearance: none;
  		appearance: none;
  		width: 15px;
  		height: 15px;
  		background: #4CAF50;
  		cursor: pointer;
  		border-radius: 50%;
  		border: 2px #000;
	}

	#volumeControl:hover::-webkit-slider-thumb {
  		background: #0b9737;
	}
</style>

<main class="app">
	<div class="header">
		<h4>Play With Me</h4>
		<p>Score: {score}</p>
    	<input type="range" id="volumeControl" min="0" max="1" step="0.1" value="0.5" on:input={changeVolume}>
	</div>
	<div class="game">
		{#each rows as row, i}
			<div class="row">
				{#each row as box, j}
					<div
						class={"box "+box}
						on:click={()=>tapped(i,j)}
					></div>
				{/each}
			</div>
		{/each}
	</div>
	{#if gameover}
		<div class="result">
			<h2>Game Over</h2>
			<p>Score: {score}</p>
			<button id="restartButton" on:click={restart}>Restart</button>
		</div>
	{/if}
</main>