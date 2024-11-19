#  <h1 align="center"> Clint vs Zombies 🤠 </h1>

Um jogo de sobrevivência onde Clint, o caubói solitário, precisa enfrentar hordas de zumbis enquanto aguarda o resgate do seu fiel companheiro, Pé de Pano, em um tempo de 35 segundos.

## 🌵 Visão Geral

Clint está cercado! Zumbis famintos se aproximam de todos os lados, e a única esperança de Clint é resistir o máximo que puder até Pé de Pano chegar com a cavalaria.  Atire nos zumbis, movimente-se estrategicamente e sobreviva o máximo que conseguir!

## 🎮 Instruções do Jogo

* **Movimento:** Use as teclas **W** (cima), **A** (esquerda), **S** (baixo) e **D** (direita) para mover Clint (🤠) pelo mapa.
* **Atirar:** Pressione a tecla **F** para atirar nos zumbis (🧟).
* **Objetivo:** Sobreviva o máximo de tempo possível, eliminando o maior número de zumbis que conseguir.

## 🧩 Executando o Jogo

Para executar o jogo, você precisa ter o WSL (Windows Subsystem for Linux) instalado em seu computador. Siga os passos abaixo:

Para executar este projeto no WSL (Windows Subsystem for Linux), siga os passos abaixo:

1. **Clone o repositório:**
   ```bash
   git clone <URL_DO_SEU_REPOSITÓRIO>
   ```

2. **Instale o WSL:**
   Abra o prompt de comando (CMD) ou o PowerShell como administrador e execute o seguinte comando:
   ```bash
   wsl install
   ```

3. **Instale o `make`:**
   Inicie o WSL digitando `wsl` no terminal e pressione Enter. Em seguida, execute o comando:
   ```bash
   sudo apt-get update && sudo apt-get install make
   ```

4. **Navegue até a pasta do projeto:**
   Dentro do WSL, navegue até o diretório onde o projeto foi clonado:
   ```bash
   cd <CAMINHO_PARA_A_PASTA>/ClintVsZombies
   ```
   Substitua `<CAMINHO_PARA_A_PASTA>` pelo caminho real para a pasta do projeto. Por exemplo:
   ```bash
   cd /mnt/c/Users/SeuUsuario/Documents/ClintVsZombies
   ```

5. **Compile e execute:**
   Execute os seguintes comandos dentro do WSL:
   ```bash
   make clean
   make
   ./ClintVsZombies.out
   ```

**Observações:**

* Certifique-se de ter os pré-requisitos necessários para compilar o projeto instalados no seu ambiente WSL (como compilador C/C++, bibliotecas, etc.).
* O comando `make clean` remove arquivos compilados anteriormente, garantindo uma compilação limpa.
* O comando `make` compila o projeto de acordo com o Makefile.
* O comando `./ClintVsZombies.out` executa o arquivo binário gerado.
* Adapte o nome do arquivo executável (`ClintVsZombies.out`) se necessário, de acordo com o nome definido no seu projeto.


## 🐎 Capturas de Tela

![menuimage](https://github.com/user-attachments/assets/bdad93ed-16df-48f6-a9bf-44dab61fdf05)
<br>
<br>
![instructionimage](https://github.com/user-attachments/assets/82112908-7eac-48b7-b668-4577a35066bf)

## 👤 Contribuidores
<table>
  <tr>
    <td align="center">
      <a href="https://github.com/gabsvelozo" title="defina o título do link">
        <img src="https://media-gru2-1.cdn.whatsapp.net/v/t61.24694-24/363073513_6898413200275191_7865365975528337527_n.jpg?ccb=11-4&oh=01_Q5AaIEtTUs03i_ZoCFPydTs-aPITAW9sl8x07m8WYNgqI2f6&oe=6748B9EF&_nc_sid=5e03e0&_nc_cat=101" width="100px;" alt="Foto Gabi"/><br>
        <sub>
          <b>Gabrielle Mastellari</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/miguelarcanjoo" title="defina o título do link">
        <img src="https://media-gru2-1.cdn.whatsapp.net/v/t61.24694-24/415174765_918350699826319_3235731444915035055_n.jpg?ccb=11-4&oh=01_Q5AaIGbOVeBv-2ZA7DvxC84sYsvrDP5EAI84CxaykYVWBGGl&oe=6748DD86&_nc_sid=5e03e0&_nc_cat=109" width="100px;" alt="Foto Miguel"/><br>
        <sub>
          <b>Miguel Arcanjo</b>
        </sub>
      </a>
    </td>
     <td align="center">
      <a href="https://github.com/vinivent" title="defina o título do link">
        <img src="https://avatars.githubusercontent.com/u/99739118?v=4" width="100px;" alt="Foto do Vinicius Ventura"/><br>
        <sub>
          <b>Vinícius Ventura</b>
        </sub>
      </a>
    </td>
</table>
