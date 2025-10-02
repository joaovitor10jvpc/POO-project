#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include <agenda/Calendar.hpp>
#include <agenda/Event.hpp>
#include <agenda/Date.hpp>
#include <agenda/Time.hpp>
#include <agenda/DateTime.hpp>
#include <agenda/DomainTypes.hpp>
#include <agenda/Tag.hpp>
#include <agenda/Exceptions.hpp>
#include <agenda/JsonStorage.hpp>

namespace {

using namespace agenda;

Date parseDate(const std::string& s) {
    int y=0, m=0, d=0; char a=0,b=0;
    std::istringstream iss(s);
    if(!(iss >> y >> a >> m >> b >> d) || a!='-' || b!='-') throw ValidationError("Data inválida YYYY-MM-DD: " + s);
    return Date{y,m,d};
}
Time parseTime(const std::string& s) {
    int h=0, m=0; char a=0;
    std::istringstream iss(s);
    if(!(iss >> h >> a >> m) || a!=':') throw ValidationError("Hora inválida HH:MM: " + s);
    return Time{h,m};
}
Duration parseDurationMinutes(const std::string& s) {
    int mins=0;
    try { mins = std::stoi(s); } catch(...) { throw ValidationError("Duração inválida (min): " + s); }
    if (mins < 0) throw ValidationError("Duração negativa");
    return Duration{std::chrono::minutes{mins}};
}
std::vector<std::string> splitCSV(const std::string& s) {
    std::vector<std::string> out; std::string token; std::istringstream iss{s};
    while(std::getline(iss, token, ',')) if(!token.empty()) out.push_back(token);
    return out;
}

void printEvent(const Event& e) {
    std::cout << "- Id: " << e.id()
              << " | Título: " << e.title()
              << " | Início: " << e.start().date().toString() << " " << e.start().time().toString()
              << " | Duração(min): " << e.duration().value.count()
              << " | Owner: " << e.ownerId();
    if(!e.description().empty()) std::cout << " | Desc: " << e.description();
    if(!e.tags().empty()) {
        std::cout << " | Tags: ";
        for(size_t i=0;i<e.tags().size();++i){ std::cout << e.tags()[i].name(); if(i+1<e.tags().size()) std::cout<<","; }
    }
    std::cout << "\n";
}

void cmdList(Calendar& cal) {
    auto es = cal.events();
    if(es.empty()){ std::cout << "(sem eventos)\n"; return; }
    for(const auto& e: es) printEvent(e);
}
void cmdListByDate(Calendar& cal, const Date& date) {
    auto es = cal.events(); bool any=false;
    for(const auto& e: es) if (e.start().date() == date) { printEvent(e); any=true; }
    if(!any) std::cout << "(sem eventos na data " << date.toString() << ")\n";
}
void cmdListByTag(Calendar& cal, const std::string& tagName) {
    auto es = cal.events(); bool any=false;
    for(const auto& e: es) {
        const auto& tags = e.tags();
        if(std::any_of(tags.begin(), tags.end(), [&](const Tag& t){ return t.name() == tagName; })) { printEvent(e); any=true; }
    }
    if(!any) std::cout << "(sem eventos com tag '" << tagName << "')\n";
}
void cmdViewDay(Calendar& cal, const Date& date) { cmdListByDate(cal, date); }
void cmdViewWeek(Calendar& cal, const Date& from, const Date& to) {
    if (to < from) throw ValidationError("Intervalo inválido: --to antes de --from");
    auto es = cal.events(); bool any=false;
    for(const auto& e: es) {
        const auto d = e.start().date();
        if (!(d < from) && !(to < d)) { printEvent(e); any=true; }
    }
    if(!any) std::cout << "(sem eventos no intervalo " << from.toString() << " .. " << to.toString() << ")\n";
}
void cmdViewMonth(Calendar& cal, int y, int m) {
    auto es = cal.events(); bool any=false;
    for(const auto& e: es) {
        const auto& d = e.start().date();
        if (d.year() == y && d.month() == m) { printEvent(e); any=true; }
    }
    if(!any) std::cout << "(sem eventos em " << y << "-" << (m<10?"0":"") << m << ")\n";
}

void runDemo(Calendar& cal) {
    std::cout << "[DEMO] Criando eventos de exemplo...\n";
    static Id nextEventId=1, nextTagId=1;
    auto addEvent = [&](std::string title, std::string date, std::string time,
                        int durMin, Id owner, std::string desc, std::vector<std::string> tags) {
        Event e{nextEventId++, std::move(title),
                DateTime{parseDate(date), parseTime(time)},
                Duration{std::chrono::minutes{durMin}},
                owner};
        e.setDescription(std::move(desc));
        for(auto& name: tags) e.addTag(Tag{nextTagId++, name});
        cal.addEvent(e);
    };
    addEvent("Daily standup", "2025-09-25", "09:30", 15, 1, "Time A", {"trabalho"});
    addEvent("Almoço",       "2025-09-25", "12:00", 60, 1, "", {"pessoal"});
    addEvent("Revisão POO",  "2025-09-26", "10:00", 90, 2, "Etapa 2", {"trabalho","importante"});

    std::cout << "\n[DEMO] Todos os eventos:\n"; cmdList(cal);
    std::cout << "\n[DEMO] Dia 2025-09-25:\n";   cmdViewDay(cal, parseDate("2025-09-25"));
    std::cout << "\n[DEMO] Semana 2025-09-25..2025-09-26:\n"; cmdViewWeek(cal, parseDate("2025-09-25"), parseDate("2025-09-26"));
    std::cout << "\n[DEMO] Mês 2025-09:\n"; cmdViewMonth(cal, 2025, 9);
    std::cout << "\n[DEMO] Persistindo em demo_events.json e recarregando...\n";
    FileStorage fs; JsonSerializer js;
    fs.save("demo_events.json", cal.events(), js);
    auto loaded = fs.load("demo_events.json", js);
    Calendar cal2;
    for (auto& e : loaded) cal2.addEvent(e);
    std::cout << "[DEMO] Após load (demo_events.json):\n";
    cmdList(cal2);
    std::cout << "\n[DEMO] Concluído.\n";
}

void printHelp(const char* exe) {
    std::cout << "Agenda CLI (Etapa 2)\nUso:\n"
              << "  " << exe << " --help\n"
              << "  " << exe << " --demo\n"
              << "  " << exe << " list\n"
              << "  " << exe << " list-by-date --date YYYY-MM-DD\n"
              << "  " << exe << " list-by-tag  --tag NOME\n"
              << "  " << exe << " view-day  --date YYYY-MM-DD\n"
              << "  " << exe << " view-week --from YYYY-MM-DD --to YYYY-MM-DD\n"
              << "  " << exe << " view-month --year YYYY --month MM\n"
              << "  " << exe << " add --title T --date YYYY-MM-DD --time HH:MM --dur MIN --owner ID [--desc DESC] [--tags t1,t2]\n"
              << "  " << exe << " save --file FILE.json\n"
              << "  " << exe << " load --file FILE.json\n";
}

} // namespace

int main(int argc, char** argv) {
    using namespace agenda;

    Calendar cal;

    if(argc <= 1){ printHelp(argv[0]); return 0; }
    const std::string cmd = argv[1];
    try {
        if(cmd == "--help" || cmd == "-h"){ printHelp(argv[0]); return 0; }
        if(cmd == "--demo"){ runDemo(cal); return 0; }
        if(cmd == "list"){ cmdList(cal); return 0; }
        if(cmd == "list-by-date"){
            std::string dateStr;
            for(int i=2;i<argc;i++){ std::string a=argv[i]; if(a=="--date" && i+1<argc) dateStr=argv[++i]; }
            if(dateStr.empty()) throw ValidationError("Uso: list-by-date --date YYYY-MM-DD");
            cmdListByDate(cal, parseDate(dateStr)); return 0;
        }
        if(cmd == "list-by-tag"){
            std::string tag;
            for(int i=2;i<argc;i++){ std::string a=argv[i]; if(a=="--tag" && i+1<argc) tag=argv[++i]; }
            if(tag.empty()) throw ValidationError("Uso: list-by-tag --tag NOME");
            cmdListByTag(cal, tag); return 0;
        }
        if(cmd == "view-day"){
            std::string dateStr;
            for(int i=2;i<argc;i++){ std::string a=argv[i]; if(a=="--date" && i+1<argc) dateStr=argv[++i]; }
            if(dateStr.empty()) throw ValidationError("Uso: view-day --date YYYY-MM-DD");
            cmdViewDay(cal, parseDate(dateStr)); return 0;
        }
        if(cmd == "view-week"){
            std::string fromStr, toStr;
            for(int i=2;i<argc;i++){ std::string a=argv[i];
                if(a=="--from" && i+1<argc) fromStr=argv[++i];
                else if(a=="--to" && i+1<argc) toStr=argv[++i];
            }
            if(fromStr.empty() || toStr.empty()) throw ValidationError("Uso: view-week --from YYYY-MM-DD --to YYYY-MM-DD");
            cmdViewWeek(cal, parseDate(fromStr), parseDate(toStr)); return 0;
        }
        if(cmd == "view-month"){
            std::string ys, ms; int y=0,m=0;
            for(int i=2;i<argc;i++){ std::string a=argv[i];
                if(a=="--year" && i+1<argc) ys=argv[++i];
                else if(a=="--month" && i+1<argc) ms=argv[++i];
            }
            if(ys.empty() || ms.empty()) throw ValidationError("Uso: view-month --year YYYY --month MM");
            y = std::stoi(ys); m = std::stoi(ms);
            if(m<1 || m>12) throw ValidationError("Mês inválido (1-12)");
            cmdViewMonth(cal, y, m); return 0;
        }
        if(cmd == "add"){
            std::string title, dateStr, timeStr, durStr, desc, tagsCSV; Id owner=0;
            for(int i=2;i<argc;i++){
                std::string a=argv[i];
                if(a=="--title"&& i+1<argc) title=argv[++i];
                else if(a=="--date" && i+1<argc) dateStr=argv[++i];
                else if(a=="--time" && i+1<argc) timeStr=argv[++i];
                else if(a=="--dur"  && i+1<argc) durStr=argv[++i];
                else if(a=="--owner"&& i+1<argc) owner=static_cast<Id>(std::stoull(argv[++i]));
                else if(a=="--desc" && i+1<argc) desc=argv[++i];
                else if(a=="--tags" && i+1<argc) tagsCSV=argv[++i];
            }
            if(title.empty() || dateStr.empty() || timeStr.empty() || durStr.empty() || owner==0)
                throw ValidationError("Uso: add --title T --date YYYY-MM-DD --time HH:MM --dur MIN --owner ID [--desc DESC] [--tags t1,t2]");

            static Id nextEventId=1, nextTagId=1;
            Event e{nextEventId++, title, DateTime{parseDate(dateStr), parseTime(timeStr)}, parseDurationMinutes(durStr), owner};
            if(!desc.empty()) e.setDescription(desc);
            for(const auto& tname : splitCSV(tagsCSV)) e.addTag(Tag{nextTagId++, tname});
            const bool ok = cal.addEvent(e);
            if(!ok) { std::cout << "[WARN] Evento id existente.\n"; return 1; }
            std::cout << "[OK] Evento adicionado:\n"; printEvent(e); return 0;
        }
        if(cmd == "save"){
            std::string file;
            for(int i=2;i<argc;i++){ std::string a=argv[i]; if(a=="--file" && i+1<argc) file=argv[++i]; }
            if(file.empty()) throw ValidationError("Uso: save --file FILE.json");
            FileStorage fs; JsonSerializer js; fs.save(file, cal.events(), js);
            std::cout << "[OK] Salvo em " << file << "\n"; return 0;
        }
        if(cmd == "load"){
            std::string file;
            for(int i=2;i<argc;i++){ std::string a=argv[i]; if(a=="--file" && i+1<argc) file=argv[++i]; }
            if(file.empty()) throw ValidationError("Uso: load --file FILE.json");
            FileStorage fs; JsonSerializer js; auto loaded = fs.load(file, js);
            int added = 0;
            for (auto& e : loaded) if (cal.addEvent(e)) ++added;
            std::cout << "[OK] Carregado de " << file << " (adicionados " << added << ")\n"; return 0;
        }

        std::cerr << "Comando desconhecido: " << cmd << "\n"; printHelp(argv[0]); return 1;
    } catch(const std::exception& ex) {
        std::cerr << "[ERRO] " << ex.what() << "\n"; return 2;
    }
}