# Programação Orientada a Objetos (C++)

## 1. Objetivo geral

Desenvolver, em C++ e usando uma biblioteca gráfica (preferencialmente Qt, mas JUCE ou outro framework C++ são permitidos), uma aplicação completa cujo tema deve ser definido ou escolhido dentre quatro temas. O projeto deve demonstrar, de forma clara e justificável, a aplicação dos principais conceitos de Programação Orientada a Objetos em C++ e boas práticas modernas da linguagem (smart pointers, uso de STL, exceções, modularização...).

## 2. Temas (definir ou escolher)

1. MP3 Player

   ○ Reproduzir arquivos MP3 (ou WAV/OGG), controlar play/pause/stop, playlist, metadata (ID3), equalizador simples (3 bandas), salvar/recuperar playlists.

2. Editor de imagens SVG com polígonos

   ○ Carregar/salvar SVG, criar/editar/remover polígonos (vértices arrastáveis), operações booleanas simples (união/interseção descartável como opcional), camadas, exportar imagem raster (PNG).

3. Agenda (planner/organizer)

   ○ Eventos com horários, recorrência simples, categorias/tags, persistência (arquivo/BD simples), busca e filtragem, interface de calendário (mês/semana/dia).

4. Sistema de Gerenciamento de Tarefas Colaborativas (Kanban lite)

   ○ Quadros, listas, cartões, atribuição de usuários (local), mova cartões entre colunas, histórico de alterações, filtros e etiquetas.

Obs.: cada tema tem funcionalidades obrigatórias e opcionais. Foque em entregar todas as obrigatórias com qualidade; opcionais aumentam nota.

## 3. Requisitos

O projeto deve explorar explicitamente (documentar e demonstrar) os conceitos listados abaixo. Em cada item inclua onde e como foi usado no código (ex.: README / relatório técnico com apontamento de classes/linhas).

1. Abstração & Encapsulamento

   ○ Interfaces claras (.h / .cpp) e separação interface/implementação.

   ○ Campos privados, getters/setters com validação onde apropriado.

2. Classes e Objetos

   ○ Projeto com classes coerentes (modelo-dominio, controllers, views).

   ○ Uso apropriado de instâncias, relações e responsabilidades.

3. Herança & Polimorfismo

   ○ Pelo menos uma hierarquia com métodos virtuais (ex.: MediaPlayer → MP3Player, WavPlayer; Shape → Polygon, Circle).

   ○ Uso de classes abstratas (interfaces em C++) com métodos virtuais puros.

4. Composição vs Herança

   ○ Demonstre composição (objetos possuem outros objetos) e justifique escolha sobre herança.

   ○ Ex.: Playlist contém Track por composição; Window compõe Toolbar, Canvas.

5. Polimorfismo dinâmico (ligação tardia)

   ○ Uso de ponteiros/refs polimórficos e dynamic_cast apenas quando necessário; preferir interfaces.

6. Gerenciamento de recursos

   ○ Uso de RAII para recursos não gerenciados (arquivos, handles), smart pointers (std::unique_ptr, std::shared_ptr) onde adequado.

7. Templates e STL

   ○ Uso adequado de containers STL (std::vector, std::map, std::optional etc.) e pelo menos um uso significativo de templates (função template ou classe template reutilizável).

8. Sobrecarga de operadores

   ○ Pelo menos 1 exemplo realista (ex.: Time para agenda: operator<, operator==; Vector2D para pontos em SVG).

9. Tratamento de exceções

   ○ Exceções para erros críticos (arquivo não encontrado, formato inválido), captura adequada e mensagens de erro ao usuário.

10. Documentação técnica e UML

   ○ Diagrama de classes (UML), diagrama de sequência para um caso de uso principal, README com instruções de build e justificativas de design.

11. Build automatizado

   ○ Uso de CMake (recomendado) ou projeto Qt Creator com instruções claras.

## 4. Entregas — 3 etapas

Cada etapa é um commit ou tag identificado no repositório e acompanhado por um release/issue com instruções. Entregue também um PDF com relatório técnico quando solicitado na etapa. O commit deve ser avisado e a URL no repositório do github deve ser informada por e-mail para bidu @ ci.ufpb.br, com o título: “[POO-251-E003-1/2/3] MATRICULA” (onde 1/2/3 refere-se à etapa relacionada à mensagem).

### Etapa 1 — Principais classes e cabeçalhos (arquitetura)

● Entrega: até 25/09/2025 23h59

● 4 pontos

● O que entregar

   ○ Repositório com diretório design/ contendo:

      ■ Arquivos .h (cabeçalhos) das principais classes (mínimo: modelo-dominio, interfaces centrais, headers de persistência e GUI).

      ■ UML de classes (arquivo .png/.svg) e README do design.

      ■ CMakeLists.txt inicial (ou projeto Qt Creator) com dependências declaradas.

   ○ Relatório curto (1–2 páginas) justificando decisões de arquitetura (por que usar composição/herança, smart pointers, padrões adotados).

● Objetivo

   ○ Avaliar design, coerência de classes e se o projeto atende os requisitos POO.

● Critérios de aceitação mínimos

   ○ Todos os cabeçalhos compilam (sem implementação) em run de pré-check (ex.: cmake e gerar projeto, Makefile). Arquivos presentes e UML legível.

### Etapa 2 — Primeiro teste em terminal (CLI)

● Entrega: até 02/10/2025 23h59

● 4 pontos

● O que entregar

   ○ Implementação mínima funcional em modo texto (sem GUI obrigatória), com:

      ■ Build funcional (cmake --build .).

      ■ Executável CLI que demonstra operações-chave (ex.: para MP3 Player: listar arquivos, criar playlist, simular play; para SVG editor: criar polígonos via coordenadas, salvar arquivo; para Agenda: criar evento, listar eventos; para Kanban: criar quadro/lista/cartão).

      ■ README com instruções para rodar os testes CLI.

● Objetivo

   ○ Mostrar que o domínio e a lógica estão implementados e que as classes funcionam independentemente da GUI.

● Critérios de aceitação mínimos

   ○ CLI executa em uma opção o teste das principais funcionalidades com saída prevista; código usa smart pointers e STL adequadamente; exceções tratadas.

### Etapa 3 — Entrega final (GUI, documentação, demonstração)

● Entrega: até 06/10/2025 23h59

● 4 pontos

● O que entregar

   ○ Aplicação completa com interface gráfica (Qt/JUCE/outro) implementando as funcionalidades obrigatórias do tema escolhido.

   ○ Código completo, organizado.

   ○ Relatório técnico final contendo:

      ■ Diagrama de classes atualizado.

      ■ Mapas entre requisitos POO (item da seção 3) e onde foram atendidos (classes/métodos).

   ○ Vídeo curto (≤3min) demonstrando a aplicação (link ou embed para vídeo no repositório).

   ○ Tag no GitHub indicando versão final e instruções de build rodando em Linux/Windows.

● Critérios de aceitação mínimos

   ○ GUI funcional com operações principais funcionando; build reproduzível com instruções; documentação presente.

## 5. Requisitos mínimos por tema

### MP3 Player (mínimos obrigatórios)

● Lista/scan de diretório com arquivos de mídia.

● Play/pause/stop, seek, volume.

● Playlist CRUD (create/read/update/delete).

● Exemplo de classe: MediaPlayer (abstract), MP3Track, Playlist, Equalizer (com posicionamento de composição).

● Persistência: salvar playlists (JSON).

● CLI: criar playlist e simular play.

### Editor SVG com polígonos

● Implementar Canvas, Shape (abstrato), Polygon (editável).

● Edição: adicionar/remover vértices, arrastar pontos, seleção múltipla.

● Import/export SVG.

● Persistência: salvar projeto SVG + meta.

● CLI: criar polígono via linhas de comando e salvar.

### Agenda

● Classes: Event, Calendar, RecurrenceRule, User.

● Visualização: vista mês/semana/dia (mínimo).

● Persistência: JSON ou sqlite.

● Busca por data/tag.

● CLI: criar/listar eventos.

### Kanban lite

● Classes: Board, Column, Card, User, ActivityLog.

● Mover cartões entre colunas com histórico.

● Filtros e etiquetas.

● CLI: criar quadro e mover cartão.

## 6. Critérios de estilo e boas práticas

● Código em C++ moderno (C++11 mínimo).

● Cabeçalhos com guards (#pragma once ou include guards).

● Comentários onde necessário; documentação de API (doxygen opcional).

● Licença no repositório (MIT/Apache a escolher).

## 7. Checklist de entrega

● Escolher ou definir tema e criar issue no GitHub (link no repositório).

● Entrega Etapa 1: headers + UML + CMake (tag v1-design).

● Entrega Etapa 2: CLI funcional + scripts (tag v2-cli).

● Entrega Etapa 3: GUI completo + relatório + vídeo (tag v3-final).

● README com instruções de build e execução.

● Mapear cada requisito POO e indicar onde foi implementado.