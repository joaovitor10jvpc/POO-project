# Relatório Técnico – Etapa 1 (Design)

## Tema
Agenda (planner/organizer) com suporte a eventos, recorrência, tags, usuários e futuras visualizações (mês/semana/dia).

## Escopo da Etapa 1
Fornecer arquitetura base através de cabeçalhos C++ autocontidos, diagrama UML inicial e justificativas de decisões orientadas a objetos.

## Decisões de Design
1. Camadas: Neste estágio unificadas; posteriormente poderá haver separação em camadas (domain, persistence, ui). Optou-se por manter headers simples para clareza avaliativa.
2. Entidades vs. Value Objects: `Event`, `User` são entidades (Id estável). `Date`, `Time`, `Duration`, `Tag` (apesar de possuir Id para possível reutilização) atuam majoritariamente como value objects com semântica de comparação (<=>) para ordenação/pesquisas.
3. Composição: `Event` compõe `DateTime`, `Duration`, `RecurrenceRule (optional)`, `Tag` e lista de participantes por Id (evitando cópia de `User`). Composição simplifica ciclo de vida (RAII) e evita herança frágil.
4. Herança: Limitada a interfaces puras (`ICalendarSerializer`, `IStorage`, `ILogger`, `IObserver`, `ICommand`) onde substituição dinâmica é requerida (Princípio de Substituição de Liskov / DIP). Isto demonstra polimorfismo sem abuso de herança para compartilhamento de dados.
5. Templates: `Repository<T>` previne duplicação de repositórios específicos e utiliza containers STL apropriados (`unordered_map` para busca por Id; saída via `vector`). Fornece `findIf` genérico demonstrando polimorfismo paramétrico.
6. Operadores: Uso de `<=>` (C++20) em `Date`, `Time`, `DateTime`, `Duration` e `Tag` para ordenação consistente; prepara terreno para futuras estruturas de índice / filtros.
7. Exceções: Hierarquia dedicada (`AgendaError`, especializações) separa tipos de falhas e habilita tratamento diferenciado em UI/CLI. Evita retorno de códigos de erro.
8. Padrões: Observer para atualização de views (GUI na Etapa 3). Command (+UndoRedoStack) antecipado para operações de edição revertíveis (criação / alteração de eventos). Strategy/Bridge para (Serializer/Storage) permitindo formatos alternativos (ex.: ICS futuro) e múltiplos backends (arquivo, SQLite).
9. Smart Pointers & RAII: Ownership explícito em `UndoRedoStack` via `std::unique_ptr<ICommand>`. Restante das entidades são valores triviais, reduzindo necessidade de ponteiros dinâmicos. Evita memory leaks e facilita exception safety.
10. Extensibilidade: Adição de novos tipos de recorrência ou novos formatos de persistência não quebra código existente, respeitando OCP.

## Justificativa de Escolhas
- Evitou-se herança entre `Event` e especializações (ex.: Meeting, Task) para manter o escopo mínimo do tema. Caso seja expandido, `Event` pode tornar-se base abstrata se regras divergirem.
- `Tag` mantém um Id permitindo deduplicação futura e filtragem eficiente, ainda assim comportando-se como value object principal.
- `DateTime` preferido a `std::chrono::sys_time` por controle didático e clareza para o avaliador; no futuro pode ser adaptado.

## Riscos e Mitigações
| Risco | Impacto | Mitigação |
|-------|---------|-----------|
| Cálculo de recorrência complexo | Atraso em Etapa 2 | Implementar gerador incremental simples (Daily/Weekly) primeiro. |
| Persistência JSON manual suscetível a erro | Corrupção de dados | Usar biblioteca consolidada (nlohmann/json) na Etapa 2. |
| Undo/Redo aumenta escopo | Prazo | Implementar somente comandos essenciais (Create/DeleteEvent). |

## Próximos Passos
- Implementar EventService (aplicando regras de validação) separado de Calendar.
- Introduzir testes unitários (doctest ou Catch2) para value objects e repositório.
- Implementar recorrência (expansão de instâncias) e filtragem por data/tag.

## Conclusão
A arquitetura proposta atende aos requisitos de POO fornecendo base modular, extensível e segura, com clara distinção entre composição e herança, uso de templates, exceções e padrões de projeto.
