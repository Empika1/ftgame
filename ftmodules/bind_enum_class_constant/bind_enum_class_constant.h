#ifndef FTBIND_ENUM_CLASS_CONSTANT_H
#define FTBIND_ENUM_CLASS_CONSTANT_H

#define BIND_ENUM_CLASS_CONSTANT(m_prefix, m_class, m_constant)                                    \
    ::ClassDB::bind_integer_constant(get_class_static(),                                           \
                                     __constant_get_enum_name(m_prefix##m_class ::m_constant,      \
                                                              #m_prefix #m_class                   \
                                                              "::" #m_constant),                   \
                                     #m_class "_" #m_constant, m_prefix##m_class ::m_constant);

#endif