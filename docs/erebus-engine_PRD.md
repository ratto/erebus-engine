# PRD: Erebus Engine (Core C++)

**Versão:** 1.0  
**Data de Criação:** 16/03/2026  
**Status:** Iniciado  
**Repositório:** `github.com/org/erebus-engine`

---

## 1. Visão Geral do Produto

O **Erebus Engine Core** é o coração técnico do projeto Erebus. Trata-se de uma biblioteca C++ multiplataforma, open source e gratuita que implementa as mecânicas do **Sistema Daemon** (RPG brasileiro criado por Marcelo Del Debio).

O Core é **completamente independente de qualquer plataforma ou framework de apresentação**, permitindo sua integração em projetos Unreal Engine, Web (via WebAssembly), Android, iOS e outras plataformas via bindings e adapters.

### Missão

Fornecer uma implementação confiável, testada e reutilizável das mecânicas do Sistema Daemon, permitindo que desenvolvedores criem jogos e aplicações digitais fiéis ao sistema original, sem reinventar a roda.

---

## 2. Objetivos do Produto

### Objetivos Primários (MVP)

1. **Implementar as mecânicas base do Sistema Daemon em C++** com alta performance e sem dependências de classe ou profissão
2. **Garantir multiplataforma** via arquitetura hexagonal e CI/CD testando builds em Linux, Windows, macOS e WebAssembly
3. **Disponibilizar como submódulo Git** para consumo por projetos que dependem dele (erebus-api, plugins Unreal, etc.)
4. **Documentar implementação** com README técnico, guia de contribuição e exemplos de integração

### Objetivos Secundários (Pós-MVP)

- Suporte a plugins de terceiros via interfaces bem definidas
- Bindings para C#, Python e JavaScript
- Otimizações de performance para plataformas mobile

---

## 3. User Personas

| Persona | Descrição | Necessidades |
|---------|-----------|--------------|
| **Dev Indie C++** | Desenvolvedor independente com conhecimento em C++ que quer criar um jogo RPG baseado no Sistema Daemon | Engine confiável, bem documentada, fácil de integrar |
| **Game Studio Brasil** | Estúdio de games brasileiro buscando diferenciais de mercado | Engine de qualidade produção, suporte comunitário, licença aberta |
| **Engine Developer** | Desenvolvedor experiente com interesse em arquitetura de software | Código limpo, arquitetura hexagonal, abstrações bem definidas, fácil para estender |
| **Mestre/Criador Daemon** | Mestre de RPG ou criador de conteúdo interessado em ferramentas digitais | Engine que respeita o sistema original, documentação em português, comunidade ativa |

---

## 4. Escopo do MVP

### O que está dentro do escopo

- ✅ Implementação completa das **entidades do Sistema Daemon**: Character, Attributes, Skills, Combat, Magic
- ✅ Arquitetura Hexagonal com **Ports & Adapters** pattern
- ✅ **Event-Driven Architecture** com EventBus interno para auditoria e observabilidade
- ✅ Serialização JSON (nlohmann/json) para persistência e integração
- ✅ Testes unitários e de integração com **Google Test (gtest)**
- ✅ CI/CD com **GitHub Actions** para builds multiplataforma
- ✅ Logging estruturado com **spdlog**
- ✅ Documentação técnica (README, Wiki, exemplos de código)
- ✅ Licença open source (MIT ou Apache 2.0)

### O que está fora do escopo (MVP)

- ❌ Editor visual de fichas ou cenários
- ❌ Persistência de dados em banco (apenas serialização JSON)
- ❌ Bindings C#/Python/JavaScript (fase posterior)
- ❌ Suporte a plataformas específicas como Unreal/Unity (será feito via adapters externos)
- ❌ Documentação em vídeo ou tutoriais interativos

---

## 5. Requisitos Funcionais

### RF-1: Gerenciamento de Atributos

**Descrição:** O Core deve implementar o sistema de atributos do Sistema Daemon com seus modificadores e derivações.

**Critérios de Aceite:**
- [ ] Classe `Attribute` com pelo menos 8 atributos principais (FOR, AGI, CON, INT, PRE, VON, PHI, ELE)
- [ ] Método para calcular capacidade física via fórmula exponencial: `Y = K × 2^(Atributo / 6)`
- [ ] Suporte a atributos secundários (DEF, RES, SAC, movimento)
- [ ] Serialização/Desserialização JSON completa
- [ ] Testes unitários com cobertura > 90%

**Requisitos Técnicos:**
- Usar `nlohmann/json` para serialização
- Implementar em namespace `erebus::domain`
- Interface `IAttributeCalculator` em `erebus::ports`

---

### RF-2: Sistema de Perícias

**Descrição:** O Core deve gerenciar perícias com seus modificadores, grupos e cálculo de testes.

**Critérios de Aceite:**
- [ ] Classe `Skill` com properties: nome, descrição, atributo-base, categoria, modificadores
- [ ] Método `resolveTest()` que implementa fórmula de teste do Sistema Daemon
- [ ] Suporte a categorias de perícias (Combat, Magic, Social, Knowledge, etc.)
- [ ] Evento `SkillTested` emitido a cada teste com resultado
- [ ] Banco de dados de ~150 perícias do Sistema Daemon populável
- [ ] Testes com cobertura > 85%

**Requisitos Técnicos:**
- Implementar padrão Strategy para diferentes tipos de testes
- EventBus deve registrar cada teste com resultado detalhado
- Interface `ISkillResolver` em `erebus::ports`

---

### RF-3: Criação e Gerenciamento de Personagem

**Descrição:** O Core deve permitir criação completa de personagem segundo as regras do Sistema Daemon.

**Critérios de Aceite:**
- [ ] Classe `Character` com propriedades: nome, atributos, perícias, vida, magia, traços
- [ ] Método `generateRandom()` para criação aleatória (101 pts Realista / 111 pts Aventura)
- [ ] Método `distributePoints()` para distribuição manual de pontos
- [ ] Cálculo automático de vida, magia, defesa, resistência e movimento
- [ ] Serialização completa do personagem (salvar/carregar JSON)
- [ ] Evento `CharacterCreated` emitido com dados do personagem
- [ ] Suporte a versioning/migrações de formato

**Requisitos Técnicos:**
- Implementar Factory pattern para criação
- Interface `ICharacter` em `erebus::ports`
- Validações de regras do Sistema Daemon em cada operação

---

### RF-4: Sistema de Combate

**Descrição:** O Core deve implementar resolução de turnos de combate segundo as regras do Sistema Daemon.

**Critérios de Aceite:**
- [ ] Classe `CombatTurn` com propriedades: iniciativa, ação, defesa, resultado
- [ ] Método `resolveCombat()` que aplica regras de ataque, defesa, dano
- [ ] Suporte a tipos de ação (Attack, Defend, Cast, Dodge, Retreat)
- [ ] Cálculo de iniciativa baseado em atributos
- [ ] Cálculo de dano com influência de atributos
- [ ] Evento `CombatResolved` com detalhes da resolução
- [ ] Suporte a combate entre múltiplos combatentes

**Requisitos Técnicos:**
- Interface `ICombatEngine` em `erebus::ports`
- Implementar padrão Command para ações
- EventBus deve logar cada etapa da resolução

---

### RF-5: Event Bus e Observabilidade

**Descrição:** O Core deve emitir eventos estruturados de todas as operações para permitir logging e auditoria em tempo real.

**Critérios de Aceite:**
- [ ] Classe `EventBus` com métodos `publish()` e `subscribe()`
- [ ] Suporte a múltiplos subscribers por evento
- [ ] Eventos estruturados com timestamp, tipo, dados e origem
- [ ] Eventos principais: `CharacterCreated`, `SkillTested`, `CombatResolved`, `AttributeModified`
- [ ] Integração com spdlog para persistência de logs
- [ ] API para consumo de eventos (adapters HTTP/WebSocket)
- [ ] Exemplo de adapter SSE (Server-Sent Events) no erebus-api

**Requisitos Técnicos:**
- Usar publisher-subscriber pattern
- Events devem ser serializáveis em JSON
- Implementar em namespace `erebus::events`

---

### RF-6: Magia e Castings

**Descrição:** O Core deve implementar o sistema de magia do Sistema Daemon com Schools, Rituals e Casting Resolution.

**Critérios de Aceite:**
- [ ] Classe `Spell` com propriedades: nome, escola, efeito, custo de magia
- [ ] Classe `MagicSchool` com lista de conjurações
- [ ] Método `castSpell()` que valida pré-requisitos e resolve conjuração
- [ ] Cálculo de sucesso baseado em perícia + atributo WIL
- [ ] Suporte a ritual e casting instantâneo
- [ ] Evento `SpellCasted` com resultado
- [ ] Base de dados de ~80 magias do Sistema Daemon

**Requisitos Técnicos:**
- Interface `IMagicEngine` em `erebus::ports`
- Strategy pattern para diferentes escolas de magia
- EventBus deve registrar casting com sucesso/falha

---

## 6. Requisitos Não-Funcionais

| Requisito | Critério | Métrica |
|-----------|----------|---------|
| **Performance** | Teste de atributo deve resolver em < 10ms | Benchmark com Google Benchmark |
| **Portabilidade** | Build bem-sucedido em Linux, Windows, macOS, WASM | CI/CD com 4 targets |
| **Testabilidade** | Cobertura de testes > 80% | gcov/lcov no CI |
| **Manutenibilidade** | Código sem warnings em -Wall -Wextra | CI deve falhar com warnings |
| **Documentação** | README técnico + exemplos de código para cada port | 100% de cobertura das públicas |
| **Extensibilidade** | Fácil criar novos adapters sem modificar Core | 3+ exemplos de adapters fornecidos |

---

## 7. Arquitetura Técnica

### Estrutura de Diretórios

```
erebus-engine/
├── domain/                 # Lógica pura, sem dependências
│   ├── character.h
│   ├── attribute.h
│   ├── skill.h
│   ├── combat.h
│   ├── magic.h
│   └── ...
├── ports/                  # Interfaces abstratas
│   ├── i_character.h
│   ├── i_skill_resolver.h
│   ├── i_combat_engine.h
│   ├── i_event_bus.h
│   └── ...
├── adapters/               # Implementações concretas
│   ├── event_bus_impl.h
│   ├── json_serializer.h
│   └── ...
├── events/                 # Sistema de eventos
│   ├── event_bus.h
│   ├── event_types.h
│   └── subscribers/
├── tests/                  # Testes unitários e integração
│   ├── unit/
│   └── integration/
├── CMakeLists.txt
├── README.md
├── CONTRIBUTING.md
└── LICENSE (MIT/Apache 2.0)
```

### Stack Técnica

- **Linguagem:** C++17 (ou superior)
- **Build System:** CMake 3.20+
- **Testes:** Google Test (gtest)
- **Logging:** spdlog (header-only)
- **JSON:** nlohmann/json
- **CI/CD:** GitHub Actions
- **Distribuição:** Git submodule

### Padrões de Arquitetura

- **Hexagonal Architecture (Ports & Adapters)** para desacoplamento
- **Event-Driven Pattern** para observabilidade
- **Domain-Driven Design** para modelagem de negócio
- **Strategy Pattern** para diferentes tipos de testes/mágias
- **Factory Pattern** para criação de entidades

---

## 8. Plano de Entrega (Sprints)

### Sprint 1: Setup & Infra (1-2 semanas)

- [ ] Repositório GitHub criado e configurado
- [ ] CMake, estrutura de diretórios e CI/CD funcionando
- [ ] Interfaces base (ICharacter, ISkill, IEventBus) definidas
- [ ] Pipeline CI/CD testando 4 targets (Linux, Windows, macOS, WASM)

**DoD:** Build green em todos os targets, repositório público com licença

### Sprint 2: Sistema de Perícias (2-3 semanas)

- [ ] Classe `Skill` e `SkillTest` implementadas
- [ ] Fórmula de teste do Sistema Daemon funcionando
- [ ] 150+ perícias em estrutura de dados
- [ ] Testes unitários com cobertura > 85%
- [ ] Evento `SkillTested` emitido corretamente

**DoD:** PR review, testes passando, sem warnings de compilação

### Sprint 3: Atributos & Personagem (2-3 semanas)

- [ ] Classe `Attribute` com 8 atributos principais
- [ ] Fórmula exponencial implementada e testada
- [ ] Classe `Character` com geração aleatória e distribuição de pontos
- [ ] Serialização JSON completa
- [ ] Testes com cobertura > 90%

**DoD:** Personagem completo criável e serializável

### Sprint 4: Combate & EventBus (2-3 semanas)

- [ ] Classe `CombatTurn` e `CombatResolver` funcionando
- [ ] EventBus interno com pub/sub pattern
- [ ] Evento `CombatResolved` com detalhes completos
- [ ] Integração com spdlog
- [ ] Exemplos de adapters (SSE, WebSocket)

**DoD:** Combate simples (1v1) completo, logs estruturados emitindo

### Sprint 5: Magia & Refinement (2 semanas)

- [ ] Sistema de magia implementado
- [ ] 80+ magias no banco de dados
- [ ] Testes de todas as regras com cobertura > 80%
- [ ] Documentação técnica completa
- [ ] Release v0.1.0-mvp

**DoD:** MVP pronto, repositório com release tag, 3 exemplos de integração

---

## 9. Critérios de Aceite (DoD Global)

- ✅ Código compilando sem warnings em `-Wall -Wextra`
- ✅ Cobertura de testes > 80% (gcov)
- ✅ PR review por pelo menos um desenvolvedor
- ✅ Todos os testes passando no CI
- ✅ Documentação técnica atualizada (README, exemplos)
- ✅ Sem dependências externas desnecessárias (apenas gtest, spdlog, nlohmann/json)
- ✅ Builds multiplataforma (Linux, Windows, macOS, WASM) verdes

---

## 10. Métricas de Sucesso

| Métrica | Meta MVP | Meta Longo Prazo |
|---------|----------|------------------|
| **Cobertura de Testes** | > 80% | > 90% |
| **Build Time** | < 60s | < 120s (com Link-Time Optimization) |
| **Teste de Perícia** | < 10ms | < 5ms |
| **GitHub Stars** | 50+ | 500+ |
| **Contributors** | 3+ | 20+ |
| **Issues Abertos** | < 20 | < 50 |
| **Forks** | 10+ | 100+ |

---

## 11. Riscos e Mitigações

| Risco | Probabilidade | Impacto | Mitigação |
|-------|---------------|--------|-----------|
| Incompatibilidade ABI entre plataformas | Média | Alto | CMake com testes em CI para cada target desde o início |
| Complexidade subestimada do Core | Média | Alto | Desenvolvimento iterativo, MVP com escopo reduzido |
| Falta de contributors open source | Média | Médio | CONTRIBUTING.md claro, Discord da comunidade Daemon, divulgação ativa |
| Performance insuficiente em mobile | Baixa | Médio | Profiling com Valgrind/perf desde Sprint 2 |

---

## 12. Dependências e Integrações

### Dependências Internas

- Submódulo do **erebus-api** (via Git submodule)
- Futuros plugins Unreal/Unity

### Dependências Externas

- **gtest** (Google Test) - deve compilar junto ou via sistema
- **spdlog** - header-only, pode ser includido
- **nlohmann/json** - header-only, pode ser includido

### Integrações Pós-MVP

- Unreal Engine 5 Plugin
- Unity C# Bindings
- WebAssembly (emscripten)
- Android NDK
- iOS Framework

---

## 13. Glossário

| Termo | Definição |
|-------|-----------|
| **Sistema Daemon** | RPG brasileiro criado por Marcelo Del Debio, com mecânicas de atributos exponenciais e perícias flexíveis |
| **Atributo** | Característica fundamental do personagem (FOR, AGI, CON, INT, PRE, VON, PHI, ELE) |
| **Perícia** | Habilidade treinável que modifica testes com base em atributo-base e modificadores |
| **EventBus** | Sistema interno de pub/sub para emissão de eventos estruturados |
| **Hexagonal Architecture** | Padrão de arquitetura que separa domain (núcleo) de adapters (integrações) |
| **Port** | Interface abstrata que define contrato de um serviço (ex: ICharacter, ISkill) |
| **Adapter** | Implementação concreta de um Port para uma plataforma específica |

---

## 14. Documentação Necessária

- [ ] **README.md** - Overview, quickstart, exemplos de uso
- [ ] **CONTRIBUTING.md** - Guia para colaboradores, setup local, workflow de PR
- [ ] **ARCHITECTURE.md** - Detalhes técnicos, padrões, decisões de design
- [ ] **PORTING_GUIDE.md** - Como criar novo adapter (ex: Unreal, WebAssembly)
- [ ] **API_REFERENCE.md** - Documentação automática de classes/métodos (Doxygen)
- [ ] **EXAMPLES/** - Pasta com 3+ exemplos de uso básico

---

## 15. Revisões e Aprovações

| Papel | Nome | Assinatura | Data |
|------|------|-----------|------|
| **Product Owner** | [Nome] | ________________ | __/__/____ |
| **Tech Lead** | [Nome] | ________________ | __/__/____ |
| **Stakeholder** | Marcelo Del Debio / Daemon Editora | ________________ | __/__/____ |

---

**Próximo Review:** 30/04/2026  
**Última Atualização:** 16/03/2026
