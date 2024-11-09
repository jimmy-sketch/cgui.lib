#pragma once

namespace cgui
{

struct logicPos {
    int row, col;
};
static bool operator<(logicPos lhs, logicPos rhs) {
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.col < rhs.col);
}
static bool operator==(logicPos lhs, logicPos rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

struct Pos
{
    size_t row, col;
    size_t depth;
};
static bool operator<(Pos lhs, Pos rhs) {
    // 深的反而小
    if (lhs.depth < rhs.depth) {
        return false;
    }
    else if (lhs.depth > rhs.depth) {
        return true;
    }
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.col < rhs.col);
}
static bool operator==(Pos lhs, Pos rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col) && (lhs.depth == rhs.depth);
}

}