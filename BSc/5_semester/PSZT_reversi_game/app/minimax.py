import copy

from typing import Tuple

from .reversi import Reversi


class Minimax:
    def __init__(self):
        self.is_processing = False

    def terminate(self):
        self.is_processing = False

    def get_optimal_move(self, game_state: Reversi, depth: int) -> Tuple[int, int]:
        self.is_processing = True
        optimal_move = (-1, -1)

        if depth < 1 or game_state.is_finished:
            return optimal_move

        is_maximizing = True if game_state.current_player == 1 else False
        alpha = int(-pow(game_state.board_size, 2))
        beta = int(pow(game_state.board_size, 2))

        if is_maximizing:
            max_val = -pow(game_state.board_size, 2)
            for move in game_state.valid_moves_list:
                val = self.__minimax(game_state, move, depth - 1, alpha, beta)
                if val > max_val:
                    max_val = val
                    optimal_move = move
                    alpha = val

        else:
            min_val = pow(game_state.board_size, 2)
            for move in game_state.valid_moves_list:
                val = self.__minimax(game_state, move, depth - 1, alpha, beta)
                if val < min_val:
                    min_val = val
                    optimal_move = move
                    beta = val

        return optimal_move

    def __minimax(self, game_state: Reversi, move: Tuple[int, int], depth: int, alpha: int, beta: int) -> int:
        if not self.is_processing:
            return -1

        game = copy.deepcopy(game_state)
        assert game.make_a_move(row=move[0], col=move[1])

        if depth == 0 or game.is_finished:
            return game.player1_points - game.player2_points

        is_maximizing = True if game.current_player == 1 else False

        if is_maximizing:
            max_val = -pow(game.board_size, 2)
            for move in game.valid_moves_list:
                val = self.__minimax(game, move, depth - 1, alpha, beta)
                max_val = max(max_val, val)
                alpha = max(alpha, val)
                if beta <= alpha:
                    break
            return max_val

        else:
            min_val = pow(game.board_size, 2)
            for move in game.valid_moves_list:
                val = self.__minimax(game, move, depth - 1, alpha, beta)
                min_val = min(min_val, val)
                beta = min(beta, val)
                if beta <= alpha:
                    break
            return min_val
