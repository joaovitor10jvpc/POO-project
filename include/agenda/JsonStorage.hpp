#pragma once
#include <agenda/Storage.hpp>
#include <agenda/ICalendarSerializer.hpp>
#include <agenda/Event.hpp>
#include <agenda/Date.hpp>
#include <agenda/Time.hpp>
#include <agenda/DateTime.hpp>
#include <agenda/DomainTypes.hpp>
#include <agenda/Tag.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

namespace agenda {

static inline std::string jsonEscape(const std::string& in) {
    std::string out; out.reserve(in.size() + 8);
    for (char c : in) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

// Serializer JSON minimalista para nossos campos
class JsonSerializer : public ICalendarSerializer {
public:
    std::string serialize(const std::vector<Event>& events) const override {
        std::ostringstream oss;
        oss << "{\n  \"events\": [\n";
        for (size_t i = 0; i < events.size(); ++i) {
            const auto& e = events[i];
            oss << "    {\n";
            oss << "      \"id\": " << e.id() << ",\n";
            oss << "      \"title\": \"" << jsonEscape(e.title()) << "\",\n";
            oss << "      \"start\": { \"date\": \"" << e.start().date().toString()
                << "\", \"time\": \"" << e.start().time().toString() << "\" },\n";
            oss << "      \"duration_minutes\": " << e.duration().value.count() << ",\n";
            oss << "      \"owner_id\": " << e.ownerId() << ",\n";
            oss << "      \"description\": \"" << jsonEscape(e.description()) << "\",\n";
            oss << "      \"tags\": [";
            for (size_t ti = 0; ti < e.tags().size(); ++ti) {
                const auto& t = e.tags()[ti];
                oss << "{ \"id\": " << t.id() << ", \"name\": \"" << jsonEscape(t.name()) << "\" }";
                if (ti + 1 < e.tags().size()) oss << ", ";
            }
            oss << "]\n";
            oss << "    }";
            if (i + 1 < events.size()) oss << ",";
            oss << "\n";
        }
        oss << "  ]\n}\n";
        return oss.str();
    }

private:
    // Utilitários toscos para leitura do mesmo JSON que escrevemos (assumem formato conhecido)
    static inline void skipWS(const std::string& s, size_t& i) {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    }
    static inline bool consume(const std::string& s, size_t& i, char c) {
        skipWS(s, i);
        if (i < s.size() && s[i] == c) { ++i; return true; }
        return false;
    }
    static inline std::string parseString(const std::string& s, size_t& i) {
        skipWS(s, i);
        if (i >= s.size() || s[i] != '"') throw std::runtime_error("JSON: expected string");
        ++i;
        std::string out;
        while (i < s.size()) {
            char c = s[i++];
            if (c == '"') break;
            if (c == '\\') {
                if (i >= s.size()) break;
                char e = s[i++];
                switch (e) {
                    case '\\': out += '\\'; break;
                    case '"':  out += '"';  break;
                    case 'n':  out += '\n'; break;
                    case 'r':  out += '\r'; break;
                    case 't':  out += '\t'; break;
                    default: out += e; break;
                }
            } else {
                out += c;
            }
        }
        return out;
    }
    static inline long long parseInt(const std::string& s, size_t& i) {
        skipWS(s, i);
        bool neg = false;
        if (i < s.size() && (s[i] == '-' || s[i] == '+')) { neg = s[i] == '-'; ++i; }
        long long v = 0;
        bool any = false;
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) {
            any = true; v = v * 10 + (s[i++] - '0');
        }
        if (!any) throw std::runtime_error("JSON: expected number");
        return neg ? -v : v;
    }
    static inline void expectKey(const std::string& s, size_t& i, const char* key) {
        skipWS(s, i);
        auto k = parseString(s, i);
        skipWS(s, i);
        if (!consume(s, i, ':')) throw std::runtime_error("JSON: expected ':'");
        if (k != key) throw std::runtime_error(std::string("JSON: unexpected key '") + k + "'");
    }

public:
    std::vector<Event> deserialize(const std::string& data) const override {
        std::vector<Event> out;
        size_t i = 0;
        skipWS(data, i);
        if (!consume(data, i, '{')) throw std::runtime_error("JSON: expected '{'");
        // find "events"
        expectKey(data, i, "events");
        skipWS(data, i);
        if (!consume(data, i, '[')) throw std::runtime_error("JSON: expected '['");
        skipWS(data, i);
        // iterate objects
        bool first = true;
        while (i < data.size() && data[i] != ']') {
            if (!first) {
                if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ','");
            }
            first = false;
            skipWS(data, i);
            if (!consume(data, i, '{')) throw std::runtime_error("JSON: expected '{' (event)");
            // Parse event fields in qualquer ordem, mas assumindo nosso formato
            // id
            expectKey(data, i, "id");
            Id id = static_cast<Id>(parseInt(data, i));
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after id");
            // title
            expectKey(data, i, "title");
            std::string title = parseString(data, i);
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after title");
            // start
            expectKey(data, i, "start");
            if (!consume(data, i, '{')) throw std::runtime_error("JSON: expected '{' (start)");
            expectKey(data, i, "date");
            std::string dateStr = parseString(data, i);
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' in start");
            expectKey(data, i, "time");
            std::string timeStr = parseString(data, i);
            if (!consume(data, i, '}')) throw std::runtime_error("JSON: expected '}' (start)");
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after start");
            // duration_minutes
            expectKey(data, i, "duration_minutes");
            int durMin = static_cast<int>(parseInt(data, i));
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after duration");
            // owner_id
            expectKey(data, i, "owner_id");
            Id owner = static_cast<Id>(parseInt(data, i));
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after owner_id");
            // description
            expectKey(data, i, "description");
            std::string desc = parseString(data, i);
            if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' after description");
            // tags
            expectKey(data, i, "tags");
            if (!consume(data, i, '[')) throw std::runtime_error("JSON: expected '[' (tags)");
            std::vector<Tag> tags;
            skipWS(data, i);
            bool firstTag = true;
            while (i < data.size() && data[i] != ']') {
                if (!firstTag) {
                    if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' (tags)");
                }
                firstTag = false;
                if (!consume(data, i, '{')) throw std::runtime_error("JSON: expected '{' (tag)");
                expectKey(data, i, "id");
                Id tid = static_cast<Id>(parseInt(data, i));
                if (!consume(data, i, ',')) throw std::runtime_error("JSON: expected ',' in tag");
                expectKey(data, i, "name");
                std::string tname = parseString(data, i);
                if (!consume(data, i, '}')) throw std::runtime_error("JSON: expected '}' (tag)");
                tags.emplace_back(tid, tname);
                skipWS(data, i);
            }
            if (!consume(data, i, ']')) throw std::runtime_error("JSON: expected ']' (tags)");
            skipWS(data, i);
            if (!consume(data, i, '}')) throw std::runtime_error("JSON: expected '}' (event)");

            // Monta Event
            DateTime start{ Date{
                                std::stoi(dateStr.substr(0,4)),
                                std::stoi(dateStr.substr(5,2)),
                                std::stoi(dateStr.substr(8,2))
                            },
                            Time{
                                std::stoi(timeStr.substr(0,2)),
                                std::stoi(timeStr.substr(3,2))
                            }};
            Event ev{ id, title, start, Duration{std::chrono::minutes{durMin}}, owner };
            ev.setDescription(desc);
            for (auto& t : tags) ev.addTag(t);
            out.push_back(std::move(ev));

            skipWS(data, i);
        }
        if (!consume(data, i, ']')) throw std::runtime_error("JSON: expected ']' (events)");
        // opcional trailing campos no root
        return out;
    }
};

class FileStorage : public IStorage {
public:
    void save(const std::string& path, const std::vector<Event>& events, const ICalendarSerializer& s) override {
        std::ofstream f(path, std::ios::binary | std::ios::trunc);
        if (!f) throw std::runtime_error("Falha ao abrir arquivo para escrita: " + path);
        const auto blob = s.serialize(events);
        f.write(blob.data(), static_cast<std::streamsize>(blob.size()));
        if (!f) throw std::runtime_error("Falha ao escrever arquivo: " + path);
    }
    std::vector<Event> load(const std::string& path, const ICalendarSerializer& s) override {
        std::ifstream f(path, std::ios::binary);
        if (!f) throw std::runtime_error("Falha ao abrir arquivo para leitura: " + path);
        std::ostringstream oss;
        oss << f.rdbuf();
        return s.deserialize(oss.str());
    }
};

} // namespace agenda
