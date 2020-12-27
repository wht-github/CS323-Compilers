#ifndef SPL_Tac
#define SPL_Tac
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
using std::string;
using std::variant;
// Quadruples
enum class Operator { LABEL,
                      FUNC,
                      ASSI,
                      PLUS,
                      MINUS,
                      MUL,
                      DIV,
                      JUMP,
                      J_EQ,
                      J_NE,
                      J_LT,
                      J_LE,
                      J_GT,
                      J_GE,
                      J_RELOP,
                      RET,
                      PARAM,
                      ARG,
                      CALL,
                      READ,
                      WRITE };
class V {
   public:
    int m_isTemp;
    int m_count;

    std::string m_name;

    V(){};
    V(int count) : m_isTemp(0),
                   m_count(count),
                   m_name("v" + std::to_string(count)) {}
    V(int count, int is_temp) : m_isTemp(is_temp),
                                m_count(count),
                                m_name(((is_temp) ? "t" : "v") + std::to_string(count)) {}
    friend bool operator==(const V &lhs, const V &rhs) {
        return lhs.m_isTemp == rhs.m_isTemp && lhs.m_count == rhs.m_count;
    }
    std::string to_string() { return m_name; }
};
class Label {
   public:
    int m_count;
    std::string m_labelName;
    Label();
    Label(int count) : m_count(count),
                       m_labelName("lb_" + std::to_string(count)) {}
};
class Tac {
   public:
    Operator m_op;
    std::string m_tac;
    std::string m_relop;
    std::variant<int, V> m_arg1;
    std::variant<int, V> m_arg2;
    std::variant<Label, V, std::string, int> m_result;
    Tac(Operator op, std::string name) : m_op(op),
                                         m_result(name) {
        if (op == Operator::LABEL) {
            m_tac = "LABEL " + name + " :";
        } else if (op == Operator::FUNC) {
            m_tac = "FUNCTION " + name + " :";
        }
    }  // LABEL x :    FUNCTION x :
    Tac(Operator op, std::string functName, V result) : m_op(op),
                                                        m_result(result) {
        V v = V();
        v.m_name = functName;
        m_arg1 = std::variant<int, V>(v);
        m_tac = result.to_string() + " := CALL " + functName;
    }  // x := CALL f
    Tac(Operator op, V args, V result) : m_op(op),
                                         m_result(result),
                                         m_arg1(args) {
        m_tac = result.to_string() + " := " + args.to_string();
    }  // ASSI
    Tac(Operator op, int args, V result) : m_op(op),
                                           m_result(result),
                                           m_arg1(args) {
        m_tac = result.m_name + " := #" + std::to_string(args);
    }  // ASSI
    Tac(Operator op, V args1, V args2, V result) : m_op(op),
                                                   m_result(result),
                                                   m_arg1(args1),
                                                   m_arg2(args2) {
        std::string val;
        if (op == Operator::PLUS) {
            val = "+";
        } else if (op == Operator::MINUS) {
            val = "-";
        } else if (op == Operator::MUL) {
            val = "*";
        } else if (op == Operator::DIV) {
            val = "/";
        }
        m_tac = result.m_name + " := " + args1.m_name + " " + val + " " + args2.m_name;
    }
    Tac(Operator op, int args1, int args2, V result) : m_op(op),
                                                       m_result(result),
                                                       m_arg1(args1),
                                                       m_arg2(args2) {
        std::string val;
        if (op == Operator::PLUS) {
            val = "+";
        } else if (op == Operator::MINUS) {
            val = "-";
        } else if (op == Operator::MUL) {
            val = "*";
        } else if (op == Operator::DIV) {
            val = "/";
        }
        m_tac = result.m_name + " := " + std::to_string(args1) + " " + val + " " + std::to_string(args2);
    }
    Tac(Operator op, int args1, V args2, V result) : m_op(op),
                                                     m_result(result),
                                                     m_arg1(args1),
                                                     m_arg2(args2) {
        std::string val;
        if (op == Operator::PLUS) {
            val = "+";
        } else if (op == Operator::MINUS) {
            val = "-";
        } else if (op == Operator::MUL) {
            val = "*";
        } else if (op == Operator::DIV) {
            val = "/";
        }
        m_tac = result.m_name + " := " + std::to_string(args1) + " " + val + " " + args2.to_string();
    }
    Tac(Operator op, V args1, int args2, V result) : m_op(op),
                                                     m_result(result),
                                                     m_arg1(args1),
                                                     m_arg2(args2) {
        std::string val;
        if (op == Operator::PLUS) {
            val = "+";
        } else if (op == Operator::MINUS) {
            val = "-";
        } else if (op == Operator::MUL) {
            val = "*";
        } else if (op == Operator::DIV) {
            val = "/";
        }
        m_tac = result.m_name + " := " + args1.to_string() + " " + val + " " + std::to_string(args2);
    }
    Tac(Operator op, V args1, V args2, Label label) : m_op(op),
                                                      m_result(label),
                                                      m_arg1(args1),
                                                      m_arg2(args2) {
        std::string val;
        if (op == Operator::J_EQ) {
            val = "==";
        } else if (op == Operator::J_NE) {
            val = "!=";
        } else if (op == Operator::J_GE) {
            val = ">=";
        } else if (op == Operator::J_GT) {
            val = ">";
        } else if (op == Operator::J_LE) {
            val = "<=";
        } else if (op == Operator::J_LT) {
            val = "<";
        }
        m_tac = "IF " + args1.m_name + " " + val + " " + args2.m_name + " GOTO " + label.m_labelName;
    }

    Tac(std::string relop, V args1, V args2, Label label) : m_op(Operator::J_RELOP),
                                                            m_relop(relop),
                                                            m_result(label),
                                                            m_arg1(args1),
                                                            m_arg2(args2) {
        m_tac = "IF " + args1.m_name + " " + relop + " " + args2.m_name + " GOTO " + label.m_labelName;
    }
    Tac(Operator op, Label label) : m_op(op),
                                    m_result(label) {
        m_tac = "GOTO " + label.m_labelName;
    }  // GOTO label
    Tac(Operator op, V result) : m_op(op),
                                 m_result(result) {
        std::string val;
        if (op == Operator::RET) {
            val = "RETURN";
        } else if (op == Operator::PARAM) {
            val = "PARAM";
        } else if (op == Operator::ARG) {
            val = "ARG";
        } else if (op == Operator::READ) {
            val = "READ";
        } else if (op == Operator::WRITE) {
            val = "WRITE";
        }
        m_tac = val + " " + result.m_name;
    }
    std::string to_string() {
        std::string tac;
        switch (m_op) {
            case Operator::LABEL: {
                std::string name = std::get<std::string>(m_result);
                tac = "LABEL " + name + " :";
                break;
            }
            case Operator::FUNC: {
                std::string name = std::get<std::string>(m_result);
                tac = "FUNCTION " + name + " :";
                break;
            }
            case Operator::JUMP: {
                std::string name = std::get<Label>(m_result).m_labelName;
                tac = "GOTO " + name;
                break;
            }
            case Operator::ASSI: {
                if (m_arg1.index() == 0) {
                    tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1));
                } else if (m_arg1.index() == 1) {
                    tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name;
                }
                break;
            }
            case Operator::PLUS: {
                if (m_arg1.index() == 0 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " + #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 1 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " + #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 0 && m_arg2.index() == 1) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " + " + std::get<V>(m_arg2).m_name;
                } else {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " + " + std::get<V>(m_arg2).m_name;
                }
                break;
            }
            case Operator::MINUS: {
                if (m_arg1.index() == 0 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " - #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 1 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " - #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 0 && m_arg2.index() == 1) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " - " + std::get<V>(m_arg2).m_name;
                } else {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " - " + std::get<V>(m_arg2).m_name;
                }
                break;
            }
            case Operator::MUL: {
                if (m_arg1.index() == 0 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " * #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 1 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " * #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 0 && m_arg2.index() == 1) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " * " + std::get<V>(m_arg2).m_name;
                } else {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " * " + std::get<V>(m_arg2).m_name;
                }
                break;
            }
            case Operator::DIV: {
                if (m_arg1.index() == 0 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " / #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 1 && m_arg2.index() == 0) {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " / #" + std::to_string(std::get<int>(m_arg2));
                } else if (m_arg1.index() == 0 && m_arg2.index() == 1) {
                    m_tac = std::get<V>(m_result).m_name + " := #" + std::to_string(std::get<int>(m_arg1)) + " / " + std::get<V>(m_arg2).m_name;
                } else {
                    m_tac = std::get<V>(m_result).m_name + " := " + std::get<V>(m_arg1).m_name + " / " + std::get<V>(m_arg2).m_name;
                }
                break;
            }
            case Operator::J_RELOP: {
                if (m_arg1.index() == 0 && m_arg2.index() == 0) {
                    m_tac = "IF #" + std::to_string(std::get<int>(m_arg1)) + " " + m_relop + " #" + std::to_string(std::get<int>(m_arg2)) + " GOTO " + std::get<Label>(m_result).m_labelName;
                } else if (m_arg1.index() == 1 && m_arg2.index() == 0) {
                    m_tac = "IF " + std::get<V>(m_arg1).m_name + " " + m_relop + " #" + std::to_string(std::get<int>(m_arg2)) + " GOTO " + std::get<Label>(m_result).m_labelName;
                } else if (m_arg1.index() == 0 && m_arg2.index() == 1) {
                    m_tac = "IF #" + std::to_string(std::get<int>(m_arg1)) + " " + m_relop + " " + std::get<V>(m_arg2).m_name + " GOTO " + std::get<Label>(m_result).m_labelName;
                } else {
                    m_tac = "IF " + std::get<V>(m_arg1).m_name + " " + m_relop + " " + std::get<V>(m_arg2).m_name + " GOTO " + std::get<Label>(m_result).m_labelName;
                }
                break;
            }
            case Operator::RET: {
                if (m_result.index() == 1) {
                    m_tac = "RETURN " + std::get<V>(m_result).m_name;
                } else if (m_result.index() == 3) {
                    m_tac = "RETURN #" + std::get<int>(m_result);
                }
                break;
            }
            case Operator::PARAM: {
                m_tac = "PARAM " + std::get<V>(m_result).m_name;
                break;
            }
            case Operator::ARG: {
                if (m_result.index() == 1) {
                    m_tac = "ARG " + std::get<V>(m_result).m_name;
                } else if (m_result.index() == 3) {
                    m_tac = "ARG #" + std::get<int>(m_result);
                }
                break;
            }
            case Operator::CALL: {
                m_tac = std::get<V>(m_result).m_name + " := CALL " + std::get<V>(m_arg1).m_name;
                break;
            }
            case Operator::READ: {
                m_tac = "READ " + std::get<V>(m_result).m_name;
                break;
            }
            case Operator::WRITE: {
                if (m_result.index() == 1) {
                    m_tac = "WRITE " + std::get<V>(m_result).m_name;
                } else if (m_result.index() == 3) {
                    m_tac = "WRITE #" + std::get<int>(m_result);
                }
                break;
            }
        }
        return m_tac;
    }
};

class ICG {
   public:
    std::vector<Tac> m_genCodes;
    std::unordered_map<std::string, int> m_variable_dict;  // 保存变量名和对应的v number
    int t_count = 0;
    int v_count = 0;
    int label_count = 0;
    V new_variable() {
        return V(v_count++);
    };
    V new_variable(int count) {
        return V(count);
    };
    V new_temp_variable() {
        return V(t_count++, 1);
    };
    Label new_label() {
        return Label(label_count++);
    };
    void appendCode(Tac &tac) {
        m_genCodes.push_back(tac);
    };
    std::string get_variable_from_dict(const std::string &v_name) {
        auto ele = m_variable_dict.find(v_name);
        if (ele != m_variable_dict.end()) {
            return "v" + std::to_string(ele->second);
        } else {
            std::cout << "translator not find variable in dictionary, error!" << std::endl;
        }
    };
    int get_v_count_from_dict(const std::string &v_name) {
        auto ptr = m_variable_dict.find(v_name);
        if (ptr != m_variable_dict.end()) {
            return ptr->second;
        } else {
            std::cout << "translator not find variable in dictionary, error!" << std::endl;
        }
    }
    void register_variable(const std::string &name, int v_n) {
        m_variable_dict.insert_or_assign(name, v_n);
        return;
    }

};
extern ICG icg;
#endif
