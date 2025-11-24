// Minimal YAML-like parser for simple maps/sequences/scalars used by InstrumentHelper
// Supports a tiny subset of YAML: nested maps and sequences of scalars (strings/numbers)
// This parser is intentionally small and header-only.
// It expects UTF-8 text and indentation using spaces. Tabs are not supported.

#ifndef SIMPLE_YAML_H
#define SIMPLE_YAML_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cctype>
#include <optional>

namespace simple_yaml
{

    using NodeMap = std::map<std::string, std::string>;

    // Represent a YAML node in one of three forms: scalar, sequence, or map
    struct Node
    {
        enum class Type
        {
            Null,
            Scalar,
            Sequence,
            Map
        } type = Type::Null;
        std::string scalar;
        std::vector<std::string> sequence; // sequence of scalars
        std::map<std::string, Node> map;

        bool IsScalar() const { return type == Type::Scalar; }
        bool IsSequence() const { return type == Type::Sequence; }
        bool IsMap() const { return type == Type::Map; }
        bool IsNull() const { return type == Type::Null; }
    };

    // Very small parser: builds a Node map for the top-level mapping in the file.
    // It only supports indentation-based nesting with 2-space or more indentation levels.
    class Parser
    {
    public:
        explicit Parser(const std::string &text) : text_(text) {}

        // Parse and return the root node (should be a map)
        Node Parse()
        {
            Node root;
            root.type = Node::Type::Map;
            std::istringstream in(text_);
            std::string line;
            std::vector<std::pair<int, Node *>> stack; // (indent, node)
            stack.push_back({-1, &root});

            while (std::getline(in, line))
            {
                // trim trailing \r
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();

                // skip empty or comment lines
                auto trimmed = trim(line);
                if (trimmed.empty() || trimmed.rfind("#", 0) == 0)
                    continue;

                int indent = count_leading_spaces(line);
                // find parent level
                while (!stack.empty() && indent <= stack.back().first)
                    stack.pop_back();

                Node *parent = stack.back().second;

                // sequence item: starts with '-'
                auto p = ltrim(line);
                if (!p.empty() && p[0] == '-')
                {
                    std::string item = trim(p.substr(1));
                    // ensure parent is a sequence
                    if (parent->type != Node::Type::Sequence)
                    {
                        parent->type = Node::Type::Sequence;
                        parent->sequence.clear();
                    }
                    parent->sequence.push_back(strip_quotes(item));
                    // if the item has no further nested content, continue
                    continue;
                }

                // otherwise expect key: value or key:
                auto colonPos = p.find(':');
                if (colonPos == std::string::npos)
                {
                    // treat as scalar line
                    // attach to parent map with an auto-key? skip
                    continue;
                }

                std::string key = strip_quotes(trim(p.substr(0, colonPos)));
                std::string rest = trim(p.substr(colonPos + 1));

                Node &nodeRef = parent->map[key];
                if (rest.empty())
                {
                    // map node
                    nodeRef.type = Node::Type::Map;
                    stack.push_back({indent, &nodeRef});
                }
                else
                {
                    // scalar or inline sequence (not fully supported) -> store scalar
                    nodeRef.type = Node::Type::Scalar;
                    nodeRef.scalar = strip_quotes(rest);
                }
            }

            return root;
        }

    private:
        std::string text_;

        static int count_leading_spaces(const std::string &s)
        {
            int i = 0;
            while (i < (int)s.size() && s[i] == ' ')
                ++i;
            return i;
        }

        static std::string trim(const std::string &s)
        {
            size_t a = 0;
            while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a])))
                ++a;
            size_t b = s.size();
            while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1])))
                --b;
            return s.substr(a, b - a);
        }

        static std::string ltrim(const std::string &s)
        {
            size_t a = 0;
            while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a])))
                ++a;
            return s.substr(a);
        }

        static std::string strip_quotes(const std::string &s)
        {
            if (s.size() >= 2 && ((s.front() == '"' && s.back() == '"') || (s.front() == '\'' && s.back() == '\'')))
                return s.substr(1, s.size() - 2);
            return s;
        }
    };

    // Convenience helpers to read a file and parse
    inline std::optional<Node> ParseFile(const std::string &path)
    {
        std::ifstream in(path);
        if (!in)
            return std::nullopt;
        std::ostringstream ss;
        ss << in.rdbuf();
        Parser p(ss.str());
        return p.Parse();
    }

} // namespace simple_yaml

#endif // SIMPLE_YAML_H
