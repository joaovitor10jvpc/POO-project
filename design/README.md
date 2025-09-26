# Design – Agenda Planner (Etapa 1)

## Objetivo
Arquitetura inicial (headers) para um aplicativo de Agenda (planner/organizer) atendendo aos requisitos de POO.

## Principais Pacotes / Pastas
- `agenda` (include/agenda): Modelo de domínio, infraestrutura e contratos.
- `design`: Artefatos de design (UML, verificação de headers).

## Visão Geral das Classes
| Conceito | Classe | Papel / Observações POO |
|----------|--------|-------------------------|
| Identificadores e tipos básicos | `DomainTypes` | Centraliza aliases e pequenos value objects. |
| Data e Hora | `Date`, `Time`, `DateTime`, `Duration` | Encapsulam validação e operadores de comparação (<=>). |
| Usuário | `User` | Entidade simples. |
| Tags | `Tag` | Value object comparável. |
| Recorrência | `RecurrenceRule` | Configura frequência, intervalo, limite (count/until). |
| Evento | `Event` | Agregação principal: contém tags, participantes, recorrência opcional. Composição. |
| Consulta | `EventQueryBuilder` | Interface fluente futura para filtrar eventos. |
| Repositório genérico | `Repository<T>` | Template para armazenar entidades por Id (demonstra templates + STL). |
| Calendário | `Calendar` | Fachada / agregado para manipular eventos e notificar observers. |
| Observação | `Subject`, `IObserver` | Implementa Observer (padrão) para GUI/CLI reagir a mudanças. |
| Undo/Redo | `ICommand`, `UndoRedoStack` | Padrão Command para desfazer ações (opcional futuro). |
| Persistência | `IStorage`, `ICalendarSerializer`, `JsonSerializer`, `FileStorage` | Abstraem armazenamento vs. formato (Strategy). |
| Logger | `ILogger`, `NullLogger` | Estratégia de logging injetável. |
| Exceções | `AgendaError` e derivadas | Hierarquia para tratamento de erros. |
| GUI (placeholder) | `gui::IView`, `gui::ICalendarView`, `gui::IGUIFactory`, `gui::IInputController` | Interfaces para futura implementação Qt (Etapa 3). |

## Decisões de Arquitetura (Resumo)
1. Composição sobre herança em `Event` (possui `RecurrenceRule`, `Tag`, lista de participantes) evitando herança artificial.
2. Herança usada apenas para papéis abstratos (interfaces polimórficas): `ICalendarSerializer`, `IStorage`, `ILogger`, `IObserver`, `ICommand` — justificável por necessidade de substituição dinâmica.
3. `Repository<T>` como template para evitar duplicação e mostrar uso de templates genéricos com STL (`unordered_map`, `vector`).
4. Operadores de comparação (<=>) em `Date`, `Time`, `DateTime`, `Duration`, suporte natural a ordenação e busca.
5. RAII e Smart Pointers: Interfaces usam ponteiros inteligentes onde houver ownership (ex.: `UndoRedoStack` guarda `unique_ptr<ICommand>`). Demais entidades são value types (copiáveis) para simplificar semântica e segurança.
6. Exceções personalizadas permitem captura semântica em camadas superiores (CLI/GUI) e distinção entre validação, não encontrado e erros gerais.
7. Padrões aplicados: Observer (atualização de views), Strategy (serializers / storage / logger), Command (undo/redo), Repository (acesso a dados em memória), Builder (planejado para consultas fluentes).

## Requisitos POO Mapeados (Planejamento)
- Abstração & Encapsulamento: getters, invariantes em `Time`, `Date`; interfaces puras.
- Herança & Polimorfismo: interfaces citadas; futura substituição concreta (ex.: `JsonSerializer`).
- Composição: `Calendar` agrega `Repository<Event>`; `Event` contém coleções e objetos valor.
- Polimorfismo dinâmico: via ponteiros para interfaces (`ICommand`, `ILogger`).
- Gerenciamento de recursos: `UndoRedoStack` com `unique_ptr`; sem new/delete bruto.
- Templates/STL: `Repository<T>`, uso de `vector`, `unordered_map`, `optional`, `stack`.
- Sobrecarga de operadores: `<=>` (C++20) e `==` implícito; comparações em tipos valor.
- Exceções: Hierarquia de exceções dedicada.
- Build automatizado: `CMakeLists.txt` inicial.
- UML: ver arquivo em `uml/agenda_classes.puml` + render.

## Próximos Passos (Etapa 2)
- Implementar persistência JSON real (provavelmente nlohmann/json ou similar).
- Implementar CLI mínima (criar evento, listar, filtrar por data/tag).
- Adicionar comandos concretos para undo/redo (CreateEventCommand etc.).
- Testes unitários (Catch2 / doctest) para tipos valor e repositório.
 - Implementar recorrência expandida (geração de ocorrências) e diagrama de sequência (caso de uso "Criar Evento").
 
## Observação
O diagrama de sequência será adicionado na Etapa 2/3 (não exigido estritamente nesta entrega, mas já planejado). Este README mapeia cada requisito conforme solicitado para a Etapa 1.


