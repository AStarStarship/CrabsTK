// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_ORM_H
#define CRABS_TOOLKIT_UPDATE_ORM_H

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

namespace CT::ORM {

enum class Dialect {
  PostgreSQL,
  SQLite,
};

enum class ColumnType {
  Boolean,
  Integer,
  BigInteger,
  Real,
  Text,
  Blob,
  Timestamp,
};

enum ColumnFlags : unsigned {
  ColumnNone = 0,
  ColumnPrimaryKey = 1 << 0,
  ColumnNotNull = 1 << 1,
  ColumnUnique = 1 << 2,
  ColumnAutoIncrement = 1 << 3,
};

struct Column {
  std::string_view name;
  ColumnType type;
  unsigned flags = ColumnNone;
  std::string_view default_sql;
};

struct Schema {
  std::string_view table;
  std::span<const Column> columns;
};

inline bool IdentifierIsValid(std::string_view identifier) {
  if (identifier.empty()) return false;
  for (char item : identifier) {
    bool valid = (item >= 'a' && item <= 'z') ||
                 (item >= 'A' && item <= 'Z') ||
                 (item >= '0' && item <= '9') || item == '_';
    if (!valid) return false;
  }
  return !((identifier.front() >= '0') && (identifier.front() <= '9'));
}

inline const char* ColumnTypeName(ColumnType type, Dialect dialect) {
  switch (type) {
    case ColumnType::Boolean: return "BOOLEAN";
    case ColumnType::Integer: return "INTEGER";
    case ColumnType::BigInteger: return "BIGINT";
    case ColumnType::Real: return "REAL";
    case ColumnType::Text: return "TEXT";
    case ColumnType::Blob: return dialect == Dialect::PostgreSQL ? "BYTEA" : "BLOB";
    case ColumnType::Timestamp:
      return dialect == Dialect::PostgreSQL ? "TIMESTAMPTZ" : "TEXT";
  }
  return "BLOB";
}

inline bool SchemaIsValid(const Schema& schema) {
  if (!IdentifierIsValid(schema.table) || schema.columns.empty()) return false;
  std::size_t primary_keys = 0;
  for (std::size_t i = 0; i < schema.columns.size(); ++i) {
    const Column& column = schema.columns[i];
    if (!IdentifierIsValid(column.name)) return false;
    if (column.flags & ColumnPrimaryKey) ++primary_keys;
    if ((column.flags & ColumnAutoIncrement) &&
        column.type != ColumnType::Integer &&
        column.type != ColumnType::BigInteger)
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (schema.columns[j].name == column.name) return false;
  }
  return primary_keys <= 1;
}

inline void AppendIdentifier(std::string& sql, std::string_view identifier) {
  sql.push_back('"');
  sql.append(identifier);
  sql.push_back('"');
}

inline std::string CreateTable(const Schema& schema, Dialect dialect,
                               bool if_not_exists = true) {
  if (!SchemaIsValid(schema)) return {};
  std::string sql = "CREATE TABLE ";
  if (if_not_exists) sql += "IF NOT EXISTS ";
  AppendIdentifier(sql, schema.table);
  sql += " (";
  for (std::size_t i = 0; i < schema.columns.size(); ++i) {
    if (i) sql += ", ";
    const Column& column = schema.columns[i];
    AppendIdentifier(sql, column.name);
    sql.push_back(' ');
    if (dialect == Dialect::PostgreSQL &&
        (column.flags & ColumnAutoIncrement)) {
      sql += column.type == ColumnType::BigInteger ? "BIGSERIAL" : "SERIAL";
    } else {
      sql += ColumnTypeName(column.type, dialect);
    }
    if (column.flags & ColumnPrimaryKey) sql += " PRIMARY KEY";
    if (dialect == Dialect::SQLite && (column.flags & ColumnAutoIncrement))
      sql += " AUTOINCREMENT";
    if (column.flags & ColumnNotNull) sql += " NOT NULL";
    if (column.flags & ColumnUnique) sql += " UNIQUE";
    if (!column.default_sql.empty()) {
      sql += " DEFAULT ";
      sql += column.default_sql;
    }
  }
  sql += ");";
  return sql;
}

inline std::string Insert(const Schema& schema, Dialect dialect) {
  if (!SchemaIsValid(schema)) return {};
  std::string sql = "INSERT INTO ";
  AppendIdentifier(sql, schema.table);
  sql += " (";
  std::size_t values = 0;
  for (const Column& column : schema.columns) {
    if (column.flags & ColumnAutoIncrement) continue;
    if (values++) sql += ", ";
    AppendIdentifier(sql, column.name);
  }
  sql += ") VALUES (";
  for (std::size_t i = 0; i < values; ++i) {
    if (i) sql += ", ";
    if (dialect == Dialect::PostgreSQL) {
      sql.push_back('$');
      sql += std::to_string(i + 1);
    } else {
      sql.push_back('?');
    }
  }
  sql += ");";
  return sql;
}

inline std::string SelectAll(const Schema& schema) {
  if (!SchemaIsValid(schema)) return {};
  std::string sql = "SELECT ";
  for (std::size_t i = 0; i < schema.columns.size(); ++i) {
    if (i) sql += ", ";
    AppendIdentifier(sql, schema.columns[i].name);
  }
  sql += " FROM ";
  AppendIdentifier(sql, schema.table);
  sql.push_back(';');
  return sql;
}

}  // namespace CT::ORM
#endif
