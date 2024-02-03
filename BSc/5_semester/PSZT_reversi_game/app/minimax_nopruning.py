import copy

from typing import Tuple

from .reversi import Reversi


def get_optimal_move(game_state: Reversi, depth: int) -> Tuple[int, int]:
    optimal_move = (-1, -1)

    if depth < 1 or game_state.is_finished:
        return optimal_move

    is_maximizing = True if game_state.current_player == 1 else False

    if is_maximizing:
        max_val = -pow(game_state.board_size, 2)
        for move in game_state.valid_moves_list:
            val = minimax(game_state, move, depth - 1)
            if val > max_val:
                max_val = val
                optimal_move = move

    else:
        min_val = pow(game_state.board_size, 2)
        for move in game_state.valid_moves_list:
            val = minimax(game_state, move, depth - 1)
            if val < min_val:
                min_val = val
                optimal_move = move

    return optimal_move


def minimax(game_state: Reversi, move: Tuple[int, int], depth: int) -> int:
    game = copy.deepcopy(game_state)
    assert game.make_a_move(row=move[0], col=move[1])

    if depth == 0 or game.is_finished:
        return game.player1_points - game.player2_points

    is_maximizing = True if game.current_player == 1 else False

    if is_maximizing:
        max_val = -pow(game.board_size, 2)
        for move in game.valid_moves_list:
            val = minimax(game, move, depth - 1)
            max_val = max(max_val, val)
        return max_val

    else:
        min_val = pow(game.board_size, 2)
        for move in game.valid_moves_list:
            val = minimax(game, move, depth - 1)
            min_val = min(min_val, val)
        return min_val
