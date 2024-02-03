from app.gui.main import GUI
from app.game_state import GameState


if __name__ == "__main__":
    game_state = GameState()
    gui = GUI(game_state)

    board = [[0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 1, 2, 0, 0, 0],
             [0, 0, 0, 2, 1, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0]]

    valid_moves = [[0, 0, 0, 0, 0, 0, 0, 0],
                   [0, 0, 0, 0, 0, 0, 0, 0],
                   [0, 0, 0, -1, 0, 0, 0, 0],
                   [0, 0, -1, 0, 0, 0, 0, 0],
                   [0, 0, 0, 0, 0, -1, 0, 0],
                   [0, 0, 0, 0, -1, 0, 0, 0],
                   [0, 0, 0, 0, 0, 0, 0, 0],
                   [0, 0, 0, 0, 0, 0, 0, 0]]

    while True:
        game_state.set_board_state(board)
        game_state.set_valid_moves(valid_moves)
        gui.update()
        gui.draw()
