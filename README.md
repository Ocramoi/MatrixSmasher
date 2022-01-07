# MatrixSmasher
Trabalho final de Sistemas Operacionais I - SSC0140

*Realizado utilizando raylib e c++*

## Contributors | Participantes
<a href="https://github.com/Ocramoi"><img src="https://avatars.githubusercontent.com/u/9422402?v=4" title="Ocramoi" width="80" height="80"></a>
<a href="https://github.com/milenacsilva"><img src="https://avatars2.githubusercontent.com/u/61664263?s=460&v=4" title="milenacsilva" width="80" height="80"></a>

Marco Antônio Ribeiro de Toledo  - **11796419**\
Milena Corrêa da Silva - **11795401**

## Layout dos Arquivos

### **./data/**
Contêm os arquivos referentes ao player data, ou seja, os placares dos jogos
e as opções de jogo salvas. 

### **./resources/**
Contêm as imagens e arquivos de textos utilizados para configurar o jogo,
como por exemplo: os spritesheets das animações e a wordlist contendo
as palavras do jogo.

### **./src/components/**
Contêm os classes correpondentes aos componentes da UI do jogo, 
como por exemplo: botões, sliders, animações etc.

### **./src/scenes/**
Contêm as classes correpondentes as cenas do jogo, ou seja, cada uma 
das transições de tela do mesmo. 

* Menu é a cena inicial e dela podemos ir para o jogo em si, para o scoreboard,  
para os créditos, para as opções e até mesmo sair do jogo.
* As cenas de `ScoreBoard`, `Credits`, `GameOver` e `Options` nos permitem voltar para o menu do jogo. 
* As cenas, no geral, estão ligadas entre si e a transição entre elas é feita dentro
das próprias através do `shared_ptr<Scene>* curScene`. Saimos do jogo quando esse 
ponteiro se torna um `nullptr`.

#### **./src/main.cpp**
A `main` está responsável por configurar a janela que será utilizada para executar o
jogo, além conter o loop que desenha cada cena. 

Os vetores `drawStatic` e `drawStack` contêm os elementos de UI que seram desenhados a cada
iteração pela main. A única diferença entre eles é que os elementos em drawStatic não mudam
de estado e, por isso, são apenas apagados quando alteramos a cena e os em drawStack podem ser
sendo apagados e redesenhados a cada iteração do loop.

## Threads/Semáforos

### Jogo
Como nosso jogo tem uma lógica baseada no problema Produtor-Consumidor, 
a implementação principal das threads e do semáforo (no nosso caso, implicito no mutex), ficou
dentro da cena do jogo em si:
```cpp
./src/components/Game.cpp

void Game::draw() {
	decltype(words.size()) qnt;
	{
		lock_guard<mutex> guard{wlMutex};
		qnt = words.size();
	}
	unsigned int textPadding{10},
		tempH{textPadding};
	raylib::DrawText("Current lives: " + std::to_string(lives), 10, tempH, fontSize, raylib::Color::Green());
	tempH += fontSize + textPadding;
	raylib::DrawText("Score: " + std::to_string(points), 10, tempH, fontSize, raylib::Color::Green());
	tempH += fontSize + textPadding;
	raylib::DrawText("Words on screen: " + std::to_string(qnt), 10, tempH, fontSize, raylib::Color::Green());

	auto keyPress{GetCharPressed()};
	bool anyHits{false},
		end{false};
	while (qnt-- > 0) {
		auto temp = words.front(); words.pop();
		auto wordInteract{temp.input(keyPress)};

		anyHits |= wordInteract.first;

		if (wordInteract.second) {
			points += 100;
			continue;
		}

		temp.draw();
		temp.fall();
		if (temp.getPos().y < win->GetHeight() + 15) {
			lock_guard<mutex> guard(wlMutex);
			words.push(temp);
		}
		else --lives;

		if (lives <= 0) {
			end = true;
			break;
		}
	}

	if (keyPress && !anyHits) points *= 0.95;

	if (end) endGame();
}
```

### Animações
Utilizamos threads também para as animações, sobretudo, na hora de iterar por cada
frame no spritesheet:
```cpp
./src/components/Animation.cpp

void Animation::startLoop(bool shouldLoop) {
    auto qnt = sprite.getAmntFrames();
    animHandler = thread([&, shouldLoop, qnt] () {
        while (true) {
            if (stopThread.load()) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(frameRate));
            if (++frameCounter >= qnt && !shouldLoop) {
                frameCounter--;
                break;
            }
            frameCounter %= qnt;
        }
    });
}

[...]

void Animation::stopLoop() {
    stopThread.store(true);
    animHandler.join();
}
```

## Screenshots

<p float="left" align="center">
  <img src="./screenshots/menu.jpg" width="500" />
  <img src="./screenshots/scoreboard.jpg" width="500" />
  <img src="./screenshots/credits.jpg" width="500" />
</p>
<p float="left" align="center">
  <img src="./screenshots/options.jpg" width="500" /> 
  <img src="./screenshots/gameover.jpg" width="500" /> 
</p>

## Vídeo 
