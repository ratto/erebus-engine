# ⚙️ Erebus Engine

> _"No princípio, havia o caos — e o caos era um segfault às 3h da manhã. O ferreiro ergueu seu martelo de C++17, derramou café sobre os grimórios do CMake e forjou, nas chamas do compilador, o núcleo sobre o qual toda a realidade seria construída."_

**Erebus Engine** é o núcleo C++17 do projeto **Erebus** — uma plataforma de mecânicas de jogo que implementa o **Sistema Daemon**, o RPG de mesa brasileiro criado por Marcelo Del Debio. Este repositório é a forja de onde saem as regras: rolagem de dados, testes de perícias, combates, magias... Sem ele, a API é um scroll em branco e o frontend é apenas um bonito portal para o vazio.

Este repositório é **um dos três pilares do ecossistema Erebus**:

| Repositório     | Stack                | Responsabilidade                              |
| --------------- | -------------------- | --------------------------------------------- |
| `erebus-engine` | C++17                | **Este repositório** — núcleo de mecânicas    |
| `erebus-api`    | Node.js + TypeScript | API REST que envolve o engine                 |
| `erebus-app`    | Vue 3 + Quasar       | Interface web (SPA)                           |

O `erebus-engine` é consumido pela `erebus-api` como **submódulo Git**, montado em `erebus-api/core/`. Pense nele como a alma do feitiço — invisível para o usuário final, mas sem ela tudo que resta é fumaça e erros de segmentação.

---

## 🏰 Arquitetura

O engine segue o padrão **Hexagonal (Ports & Adapters)** — uma filosofia arquitetural onde a lógica de domínio não sabe (e nem quer saber) se está sendo chamada por uma API REST, um binário de linha de comando ou um necromante invocando processos via subprocess.

```
domain/     ← Lógica pura de jogo. Sem dependências externas. Sem surpresas.
            Namespace: erebus::domain

ports/      ← Interfaces abstratas (IDiceRoller, IEventBus, ...)
            O domínio fala com o mundo através dessas portas.
            Namespace: erebus::ports

adapters/   ← Implementações concretas das ports (EventBusImpl, ...)
            A ponte entre o ideal e o real, assim como o café entre o sono e o código.

handlers/   ← Lógica de baixo nível (geração de números aleatórios, distribuições)
            O trabalho sujo que ninguém quer fazer mas todos precisam.

services/   ← Orquestração da aplicação. Injeta dependências, coordena handlers.
            O maestro da ópera, mas sem o jaleco branco.

events/     ← Definições de eventos (DiceRolledEvent, ...)
            Cada ação do sistema emite um evento. O EventBus entrega.

main.cpp    ← Ponto de entrada CLI. Interface JSON via stdin/stdout.
            Aceita comandos, devolve resultados. Sem firulas.
```

### Fluxo de uma rolagem

```
stdin (JSON)
     │
     ▼
  main.cpp         ← desserializa o comando
     │
     ▼
 DiceService        ← valida, orquestra
     │
     ├──► DiceHandler     ← rola os dados (Mersenne Twister, seed configurável)
     │
     └──► IEventBus       ← publica DiceRolledEvent para cada dado rolado
              │
              ▼
        EventBusImpl      ← loga no stderr com formatação unicode
     │
     ▼
stdout (JSON)       ← resultados serializados, prontos para consumo
```

---

## ⚗️ Stack & Dependências

| Componente       | Versão    | Tipo             | Finalidade                        |
| ---------------- | --------- | ---------------- | --------------------------------- |
| C++              | 17        | Linguagem        | Features modernas sem riscos arcanos |
| CMake            | `^3.20`   | Build            | Compilação multiplataforma        |
| Google Test      | `1.14.0`  | Teste            | Testes unitários e de integração  |
| Google Mock      | (incluso) | Teste            | Test doubles                      |
| nlohmann/json    | `3.11.3`  | Serialização     | Interface JSON stdin/stdout       |

**Sem dependências de runtime além da stdlib C++.** O engine compila limpo, com `-Wall -Wextra`, e qualquer warning é tratado como o que é: uma premonição de desastre.

As dependências são baixadas automaticamente via **FetchContent** no primeiro build. Nenhuma instalação manual de bibliotecas — porque a vida já é suficientemente complicada.

---

## 🏗️ Começando (Setup Local)

Antes de forjar seu primeiro dado virtual, certifique-se de ter as ferramentas do ofício:

- **CMake** `^3.20`
- **GCC** `^11` ou **Clang** `^14` (ou qualquer compilador que respeite C++17 e o seu sono)
- **Git**
- **Café** — não é opcional. O sistema vai saber.

### 1. Clone o repositório

```bash
git clone https://github.com/ratto/erebus-engine.git
cd erebus-engine
```

### 2. Configure o build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

Na primeira execução, o CMake vai baixar Google Test e nlohmann/json. É uma boa hora para encher a xícara.

### 3. Compile

```bash
cmake --build build --config Release
```

O artefato principal é o binário `build/erebus` e a biblioteca estática `liberebus_core.a`.

### 4. Execute os testes

```bash
ctest --test-dir build
```

Verde = honra e glória. Vermelho = mais café e `git blame`.

### 5. (Opcional) Build com cobertura de código

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build
ctest --test-dir build
```

Requer `gcov` e `gcovr` instalados. O relatório de cobertura aparece após os testes.

---

## ⚔️ Usando o CLI

O engine expõe uma interface de linha de comando via **JSON stdin/stdout**. Envie um comando, receba um resultado. Simples como rolar um d6 — exceto que funciona.

### Rolar dados

```bash
echo '{"command": "dice.roll", "diceType": "d6", "count": 3}' | ./build/erebus
```

```json
{"results": [4, 2, 5], "total": 11}
```

### Tipos de dados suportados

| Tipo   | Range     | Observação                          |
| ------ | --------- | ----------------------------------- |
| `d3`   | 1–3       | O primo esquecido da família        |
| `d4`   | 1–4       | O mais traiçoeiro no chão de masmorras |
| `d6`   | 1–6       | O clássico. O pai de todos.         |
| `d8`   | 1–8       | Para quando o d6 não resolve        |
| `d10`  | 1–10      | O favorito dos sistemas percentuais |
| `d12`  | 1–12      | Raramente usado, profundamente amado |
| `d100` | 0–99      | Rola de 0 a 99. O Sistema Daemon é assim. |

### Calcular capacidade física (Y)

```bash
echo '{"command": "capacity.calculate_y", "k": 5.0, "attribute": 12}' | ./build/erebus
```

```json
{"y": 17.31}
```

### Calcular K a partir de Y

```bash
echo '{"command": "capacity.calculate_k", "y": 17.31}' | ./build/erebus
```

```json
{"k": 5.0}
```

### Calcular bônus de dano

```bash
echo '{"command": "capacity.damage_bonus", "fr": 16}' | ./build/erebus
```

```json
{"damageBonus": 1}
```

> A fórmula é `ceil((FR − 13.5) / 2)`, mínimo 0. FR ≤ 13 retorna 0.

### Validar perícia de combate

```bash
echo '{
  "command": "combat_skill.validate",
  "skillType": "melee",
  "attackBase": 10,
  "defenseBase": 10,
  "pointsAllocated": 5,
  "attackPoints": 3,
  "defensePoints": 2,
  "isDefault": false
}' | ./build/erebus
```

```json
{"valid": true, "attackValue": 13, "defenseValue": 12, "errors": []}
```

**Tipos de perícia suportados:** `melee`, `ranged`, `shield`

---

## 🎲 Domínio Implementado (Sprint 3)

O engine está em desenvolvimento ativo. O que já existe:

### ✅ Sistema de Dados
- Rolagem de qualquer tipo de dado (d3 a d100)
- Múltiplos dados em uma chamada (`count > 1`)
- Geração determinística via Mersenne Twister (seed configurável para testes)
- Eventos emitidos para cada dado individualmente via EventBus

### ✅ Sistema de Capacidade Física
- Cálculo de Y (capacidade física) via fórmula `Y = K × 2^(Atributo / 6)`
- Cálculo inverso de K a partir de Y e atributo
- Cálculo de bônus de dano baseado em FR: `ceil((FR - 13.5) / 2)`, mínimo 0

### ✅ Validação de Perícias de Combate
- Validação de distribuição de pontos em perícias de combate (melee, ranged, shield)
- Regras de ataque/defesa para cada tipo de perícia
- Perícia padrão "Briga" com valores mínimos garantidos

### ⏳ Planejado (próximas iterações)

| Funcionalidade           | Status      |
| ------------------------ | ----------- |
| Criação de Personagens   | Em breve    |
| Rolagem de Testes        | Em breve    |
| Resolução de Combate     | Em breve    |
| Sistema de Magia         | Em breve    |
| Serialização JSON completa | Em breve  |

**Atributos do Sistema Daemon** (quando implementados):

| Código  | Atributo           | Descrição                                              |
| ------- | ------------------ | ------------------------------------------------------ |
| `FR`    | Força              | Capacidade muscular bruta e força física               |
| `DEX`   | Destreza           | Coordenação motora, precisão e habilidade manual       |
| `AGI`   | Agilidade          | Velocidade de reação, equilíbrio e mobilidade          |
| `CON`   | Constituição       | Resistência física, vitalidade e saúde geral           |
| `INT`   | Inteligência       | Raciocínio, memória, aprendizado e resolução de problemas |
| `WILL`  | Força de Vontade   | Determinação, concentração e resistência mental        |
| `CAR`   | Carisma            | Personalidade, poder de persuasão e liderança natural  |
| `PER`   | Percepção          | Acuidade sensorial, atenção aos detalhes e intuição    |

Fórmula de capacidade física: `Y = K × 2^(Atributo / 6)` — porque nada diz "RPG brasileiro hardcore" como uma exponencial de base 2.

---

## 🧪 Testes

O projeto usa **Google Test** com 47 testes unitários cobrindo os subsistemas de dados, capacidade física e perícias de combate. Cada teste é uma runa gravada a fio de espada no mármore da confiabilidade.

```bash
# Rodar todos os testes
ctest --test-dir build

# Rodar com output verboso (para quando as runas falham)
ctest --test-dir build --output-on-failure
```

### Organização

```
tests/
├── unit/
│   ├── dice_test.cpp               # 21 testes: domínio, handlers, services, eventos
│   ├── capacity_handler_test.cpp   # 14 testes: calcular Y, K e bônus de dano
│   ├── combat_skill_handler_test.cpp # 11 testes: melee, ranged, shield, Briga
│   └── smoke_test.cpp              # Smoke test de sanidade
└── integration/
    └── smoke_test.cpp              # Integração ponta a ponta (expandindo)
```

### O que é testado

| Suite                          | Testes | O que valida                                      |
| ------------------------------ | ------ | ------------------------------------------------- |
| `DiceRoll::total()`            | 3      | Soma correta de vetores de resultados             |
| `DiceHandler ranges`           | 7      | Min/max de cada tipo de dado (500 iterações)      |
| `DiceService count`            | 3      | Contagem padrão, múltiplos, consistência          |
| `DiceService events`           | 5      | Publicação, índices, totalCount, tipo e valores   |
| `DiceService validation`       | 3      | Rejeição de count ≤ 0, sem eventos em falha       |
| `CapacityHandler`              | 14     | Fórmula Y, inverso K, bônus de dano por FR        |
| `CombatSkillHandler`           | 10     | Melee/ranged/shield válidos, Briga padrão, erros  |

**Test double `SpyEventBus`:** implementa `IEventBus` e captura eventos publicados para asserção. Sem mocks mágicos que mentem — só injeção de dependência honesta.

---

## 🔨 Comandos de Build

```bash
# Debug (padrão para desenvolvimento)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release (para uso em erebus-api como submodule)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Executar testes
ctest --test-dir build

# Build + cobertura
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build && ctest --test-dir build
```

---

## 🗺️ Integração com erebus-api

O `erebus-engine` é consumido pela `erebus-api` como submódulo Git montado em `erebus-api/core/`. A API invoca o binário `erebus` via subprocess, troca dados por JSON e expõe tudo como endpoints REST.

Para inicializar o engine dentro da API:

```bash
cd erebus-api
git submodule update --init --recursive
cd core
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Após isso, o binário `erebus-api/core/build/Release/erebus` estará disponível para o Core Adapter da API.

---

## 🤝 Contribuindo

Este projeto está em desenvolvimento ativo, e contribuições são bem-vindas — especialmente as que chegam acompanhadas de testes, sem warnings de compilação e, idealmente, sem comentários do tipo `// funciona não sei por quê`.

1. Fork o repositório
2. Crie sua branch: `git checkout -b feat/novo-encantamento`
3. Commit suas mudanças: `git commit -m 'feat: implementar teste de perícia'`
4. Push para a branch: `git push origin feat/novo-encantamento`
5. Abra um Pull Request

**Regras de ouro:**
- O build deve passar com `-Wall -Wextra` sem warnings
- Novos subsistemas precisam de testes unitários
- Nenhum `printf` de debug esquecido em produção (o EventBus existe por um motivo)

---

## ☕ Apoie o Projeto

Se este engine te ajudou a resolver um d20 às 2h da manhã, a entender arquitetura hexagonal enquanto depurava um segfault ou simplesmente te fez companhia enquanto o CMake baixava dependências — considere apoiar o desenvolvimento.

Cada contribuição vai diretamente para o fundo de café da forja (e, ocasionalmente, para um servidor menos assombrado por memory leaks).

[![Doe via PayPal](https://img.shields.io/badge/Doe-PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://www.paypal.com/donate/?hosted_button_id=8RE442ASFC2PS)

---

## 📄 Licença

Este projeto está sob a licença **GNU General Public License v2.0**. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

O conteúdo de mecânicas de jogo é derivado do **Sistema Daemon**, criado por Marcelo Del Debio e publicado pela Daemon Editora. Veja [DaemonLicense-pt.md](DaemonLicense-pt.md) (português) e [DaemonLicense-en.md](DaemonLicense-en.md) (inglês) para os termos de uso do sistema.

---

_Forjado com ⚙️, ☕ e uma quantidade clinicamente irresponsável de C++ estritamente compilado._
