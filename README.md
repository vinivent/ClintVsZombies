# Clint vs Zombies

Um jogo de sobrevivência onde Clint, o caubói solitário, precisa enfrentar hordas de zumbis enquanto aguarda o resgate do seu fiel companheiro, Pé de Pano.

## Visão Geral

Clint está cercado! Zumbis famintos se aproximam de todos os lados, e a única esperança de Clint é resistir o máximo que puder até Pé de Pano chegar com a cavalaria.  Atire nos zumbis, movimente-se estrategicamente e sobreviva o máximo que conseguir!

## Instruções do Jogo

* **Movimento:** Use as teclas **W** (cima), **A** (esquerda), **S** (baixo) e **D** (direita) para mover Clint (🤠) pelo mapa.
* **Atirar:** Pressione a tecla **F** para atirar nos zumbis (🧟).
* **Objetivo:** Sobreviva o máximo de tempo possível, eliminando o maior número de zumbis que conseguir.

## Executando o Jogo

Para executar o jogo, você precisa ter o WSL (Windows Subsystem for Linux) instalado em seu computador. Siga os passos abaixo:

1. **Abra o prompt de comando (CMD) ou o PowerShell.**
2. **Navegue até o diretório onde o jogo está localizado.**  Por exemplo, se a pasta do jogo estiver na sua área de trabalho, use os seguintes comandos:
   ```bash
   cd Desktop
   cd ClintVsZombies
   ```
3. **Inicie o WSL:** Digite `wsl` e pressione Enter.
4. **Compile o jogo:** No terminal WSL, execute o comando `make`.  
    * **Se você não tiver o `make` instalado:** Você precisará instalá-lo.  No Ubuntu/Debian, você pode usar o seguinte comando: `sudo apt-get install make`.  Adapte o comando para sua distribuição Linux, se necessário.
5. **Execute o jogo:** Após a compilação bem-sucedida, execute o jogo com o comando `./ClintVsZombies.out`.

## Pré-requisitos

* **WSL (Windows Subsystem for Linux):**  Certifique-se de ter o WSL instalado e configurado corretamente.  Você pode encontrar instruções de instalação no site da Microsoft.
* **Make:** Certifique-se de ter o utilitário `make` instalado na sua distribuição WSL.
* **Compilador C/C++:** O código-fonte do jogo provavelmente está em C ou C++, então você precisará de um compilador compatível instalado no WSL (como o GCC).

## Capturas de Tela

![Screenshot 1](caminho/para/screenshot1.png)
![Screenshot 2](caminho/para/screenshot2.png)

## Contribuidores
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



