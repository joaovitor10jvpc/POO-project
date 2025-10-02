# Agenda Planner

Projeto de Agenda (Planner/Organizer) em C++ moderno. Etapa 1 (design) concluída e Etapa 2 (CLI) pronta para avaliação.

## Status
- v1-design: cabeçalhos, UML e build inicial para validação de arquitetura.
- v2-cli: CLI funcional com criação/listagem, busca por data/tag, visualizações (dia/semana/mês) e persistência JSON.

## Build rápido (pré-check de headers)
```bash
mkdir -p build && cd build
cmake .. -DAGENDA_BUILD_CLI=OFF
cmake --build .
```
Isso compila `agenda_header_check` garantindo que todos os headers sejam autocontidos.

## CLI (Etapa 2)
Para compilar e executar o CLI:
```bash
mkdir -p build && cd build
cmake .. -DAGENDA_BUILD_CLI=ON
cmake --build .
./agenda_cli --demo
```

Comandos principais:
```bash
# ajuda
./agenda_cli --help

# criar e listar eventos
./agenda_cli add --title "Reuniao" --date 2025-09-25 --time 10:00 --dur 60 --owner 1 --tags trabalho,importante
./agenda_cli list
./agenda_cli list-by-date --date 2025-09-25
./agenda_cli list-by-tag  --tag trabalho

# visualizações (mês/semana/dia)
./agenda_cli view-day   --date 2025-09-25
./agenda_cli view-week  --from 2025-09-25 --to 2025-09-30
./agenda_cli view-month --year 2025 --month 09

# persistência JSON
./agenda_cli save --file agenda.json
./agenda_cli load --file agenda.json
```

Demo automatizada da Etapa 2:
```bash
bash scripts/cli-demo.sh
```

### Pre-check automatizado (critério de aceitação Etapa 1)
Para reproduzir o verificador de headers:
```bash
bash scripts/precheck.sh
```
Esse script cria um build isolado (`build-precheck/`) e compila apenas o alvo `precheck_headers`.

No GitHub, o workflow `.github/workflows/precheck.yml` executa isso a cada push.

## Estrutura
- `include/agenda`: headers do domínio e infra.
- `src/main_cli.cpp`: implementação do CLI da Etapa 2.
- `design/uml`: diagrama PlantUML (`agenda_classes.puml`).
- `design/header_check.cpp`: TU para checar compilação das interfaces.
- `scripts/cli-demo.sh`: script que compila e roda a demo da Etapa 2.

## Requisitos de POO atendidos (resumo)
- Encapsulamento e composição nas entidades (`Event`, `Calendar`).
- Interfaces e polimorfismo para persistência (`ICalendarSerializer`, `IStorage`), com implementação JSON (`JsonSerializer`, `FileStorage`).
- STL e operadores: containers, `std::chrono`, operadores de comparação em `Date` e `Time`.
- Exceções: validações e mensagens claras no CLI.

Mapeamento detalhado no `design/README.md`.

## Publicação da Etapa 2 (checklist)
1) Gerar build e validar CLI localmente (com `--demo`).
2) Atualizar/confirmar este README com comandos de uso (feito).
3) Criar tag `v2-cli` e publicar no GitHub:
```bash
git add -A
git commit -m "Etapa 2: CLI com persistência JSON e visualizações"
git tag -a v2-cli -m "Etapa 2 (CLI)"
git push origin main --tags
```
4) Criar uma issue no GitHub intitulada "[POO-251-E003-2] MATRICULA" contendo:
	- Link da tag `v2-cli`.
	- Comandos para build/execução do CLI (copiar da seção acima).
	- Observações (ex.: persistência JSON e visualizações).
5) Enviar o e-mail solicitado no enunciado com a URL do repositório e tag.

## Próximas etapas (Etapa 3 – GUI)
Integrar GUI com Qt (já previsto no `CMakeLists.txt` via `AGENDA_BUILD_GUI`). A CLI permanece como base de testes.

## Licença
MIT.
