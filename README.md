# Agenda Planner

Etapa 1 (Design) – Projeto de Agenda (Planner/Organizer) em C++.

## Status
Esta tag (v1-design) contém apenas cabeçalhos, UML e build inicial para validação de arquitetura.

## Build Rápido
```bash
mkdir -p build && cd build
cmake .. -DAGENDA_BUILD_CLI=OFF
cmake --build .
```
Isso compila `agenda_header_check` garantindo que todos os headers sejam autocontidos.

### Pre-check automatizado (critério de aceitação Etapa 1)
Para reproduzir exatamente o que o avaliador precisa ("todos os cabeçalhos compilam"):
```bash
bash scripts/precheck.sh
```
Esse script cria um build isolado (`build-precheck/`) e compila apenas o alvo `precheck_headers`.

No GitHub, o workflow `.github/workflows/precheck.yml` executa isso a cada push.

## Estrutura
- `include/agenda`: headers do domínio e infra.
- `design/uml`: diagrama PlantUML (`agenda_classes.puml`).
- `design/header_check.cpp`: TU para checar compilação das interfaces.

## Requisitos de POO Atendidos (Planejamento)
Ver `design/README.md` para tabela detalhada de mapeamento.

## Próximas Etapas
Adicionar implementação do domínio + CLI (criar/listar eventos, filtragem simples) e persistência JSON real.

## Licença
MIT.
