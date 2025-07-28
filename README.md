# F1-Gallery
<p align="center">
  <img width="100px" src="https://assecom.ufersa.edu.br/wp-content/uploads/sites/24/2014/09/PNG-bras%C3%A3o-Ufersa.png" alt="Brasão da Ufersa" />
</p>

<h1 align="center"> 🏎️ F1 - Gallery 🏎️</h1>
<p align="center"><b>Projeto do Laboratório de Algoritmos e Estruturas de Dados 1</b></p>

<div align="center">

![Status](https://img.shields.io/badge/Status-Finalizando-green)
![Linguagem C](https://img.shields.io/badge/Linguagem-C-blue.svg?logo=c&logoColor=white)
![Suporte](https://img.shields.io/badge/Suporte-Windows%20%7C%20Linux-blue)

</div>

<br>

<h2> Usuário e Senha </h2>

Usuário: admin
<br>
Senha: 12345

<h2>📄 Sobre o Projeto</h2>
Aplicação que tem a função de armazenar informações de pilotos, permitindo que o usuário registre pilotos e suas estatísticas, incluindo funções para comparar bem como editar e excluir pilotos inseridos.

<br>

<h2>📝 Funcionalidades</h2>
  
- Cadastrar Piloto           
- Cadastrar Estatisticas do Piloto 
- Listar Pilotos cadastrados 
- Comparar Pilotos           
- Modo temporada (N/I)       
- Exibir estatisticas da temporada (N/I) 
- Campeoes                   
- Estatisticas no pista (N/I) 
- Editar Piloto              
- Excluir Piloto            
- Ordenar Pilotos por Nome  
- Buscar Piloto por Nome (Binaria) 
- Sair                       

<br>

## 🏗️ Estrutura do Projeto  
```shell
.
├── apps
│   ├── main.c
│   └── programa
├── include
│   ├── arquivos.h
│   ├── buscas_ordenacao.h
│   ├── pilotos_estatisticas.h
│   └── uteis.h
└── src
    ├── arquivos.c
    ├── buscas_ordenacao.c
    ├── pilotos_estatisticas.c
    └── uteis.c
```
---
<br>

## 💻 Como Compilar e Executar

### 🗹 Requisitos  
- GCC instalado (MinGW para Windows, GCC padrão no Linux)  
- Terminal/Prompt de comando

---

## 🪟 Windows
1. Abra o PowerShell ou Prompt de Comando (cmd)
2. Navegue até a pasta do projeto:
```
cd C:\Users\seunome\F1-GALLERY
```
3. Compile o projeto:
```
gcc apps/main.c src/*c -Iinclude -o apps/programa
```
4. Execute o programa:
```
/apps/programa
```
---

## 🐧 Linux
1. Abra o terminal
2. Navegue até a pasta do projeto:
```
cd F1_GALLERY/
```
3. Compile o projeto:
```
gcc apps/main.c src/*c -Iinclude -o apps/programa
```
4. Execute o programa:
```
/apps/programa
```
<br>


