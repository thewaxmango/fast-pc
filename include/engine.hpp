#ifndef ENGINE
#define ENGINE

#include <iostream>
#include <rule.hpp>

namespace engine {
    void disp_board(rule::pcstate pcstate) {
        for (int i = 0; i < rule::width * pcstate.lines; ++i) {
            std::cout << (pcstate.board & (1 << i) ? "██" : "  ");
            if (i % rule::width == 0)
                std::cout << '\n';
        }
    };

    void disp_state(rule::pcstate pcstate, rule::piecestate piecestate) {
        rule::mask piecemask = rule::piecemasks[piecestate.pc][piecestate.dir] << piecestate.os;
        for (int i = 0; i < rule::width * pcstate.lines; ++i) {
            if (pcstate.board & piecemask & (1 << i))
                std::cout << "XX";
            else if (pcstate.board & (1 << i))
                std::cout << "██";
            else if (piecemask & (1 << i))
                std::cout << "<>";
            else
                std::cout << "  ";
            if (i % rule::width == 0)
                std::cout << '\n';
        }
    };

    inline bool collides(rule::pcstate pcstate, rule::piecestate piecestate) {
        return pcstate.board & (rule::piecemasks[piecestate.pc][piecestate.dir] << piecestate.os);
    }

    inline rule::mask apply_offset(rule::mask mask, rule::offset offset) {
        return offset < 0 ? mask >> (-offset) : mask << offset;
    }

    inline rule::mask place(rule::mask board, rule::piecestate piecestate) {
        return board | (rule::piecemasks[piecestate.pc][piecestate.dir] << piecestate.os);
    }

    inline rule::mask clear_lines(rule::pcstate pcstate) {
        rule::mask newboard = 0;
        for (int i = pcstate.lines - 1; i >= 0; --i) {
            rule::mask rowmask = rule::fullrow << (i * rule::width);
            if (pcstate.board & rowmask == rowmask) {
                --pcstate.lines;
            } else {
                newboard <<= rule::width;
                newboard |= pcstate.board & rowmask;
            }
        }
        pcstate.board = newboard;
    }

    rule::mask validinput(rule::offset inputoffset, rule::mask board, rule::mask validprior, rule::piece piece, rule::direction finaldir) {
        rule::mask invboard = ~board;
        rule::mask result = validprior;
        for (rule::offset minooffset : rule::minooffsets[piece][finaldir]) {
            result &= apply_offset(invboard, inputoffset - minooffset);
        }
        return result;
    }

    rule::inputallowed gen_inputallowed(rule::mask board, rule::piece piece, rule::direction direction) {
        rule::inputallowed result;

    };

    std::array<rule::inputallowed, 4> gen_ias(rule::mask board, rule::piece piece);
    std::array<rule::mask, 4> gen_placements(rule::pcstate boardstate, rule::piece piece);
    std::vector<rule::input> gen_path(rule::mask board, rule::piece piece, rule::direction direction, rule::offset offset);
}

#endif