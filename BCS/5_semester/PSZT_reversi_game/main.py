from app.gui.main import GUI
from app.reversi import Reversi
from app.game_state import GameState, Player, State, Property
from app.minimax import Minimax
from time import sleep
import threading


class App:
    def __init__(self):
        self.game_state = GameState()
        self.gui = GUI(self.game_state)
        self.reversi = Reversi()
        self.ai = Minimax()
        self.ai_thread = None

    def run(self):
        while True:
            self.gui.update()
            if self.game_state.get_state() == State.RESTARTING:
                self.__restart()
            if not self.reversi.is_finished and not self.ai.is_processing:
                self.__make_move()
            self.gui.draw()

    def __restart(self):
        self.gui.game_timer.reset()
        if self.ai.is_processing:
            self.ai.terminate()
            self.ai_thread.join()
        self.reversi.reset()
        self.__update_game_state()
        self.gui.update()
        self.gui.game_timer.start()
        self.game_state.start()

    def __make_move(self):
        current_player = self.game_state.get_current_player()
        is_human = self.game_state.get_player_property(current_player, Property.IS_HUMAN)
        if is_human:
            self.__make_move_human()
        else:
            self.ai.is_processing = True
            self.ai_thread = threading.Thread(target=self.__make_move_ai, args=(current_player,))
            self.ai_thread.start()

    def __make_move_ai(self, current_player: Player):
        depth = self.game_state.get_player_property(current_player, Property.DEPTH)
        move = self.ai.get_optimal_move(self.reversi, depth)
        sleep(1)
        assert self.reversi.make_a_move(move[0], move[1])
        self.__update_game_state()
        self.ai.is_processing = False

    def __make_move_human(self):
        for move in self.game_state.get_moves():
            if self.reversi.make_a_move(move.row, move.col):
                break
        self.game_state.clear_moves()
        self.__update_game_state()

    def __update_game_state(self):
        self.game_state.set_current_player(Player(self.reversi.current_player))
        self.game_state.set_board_state(self.reversi.board)
        self.game_state.set_valid_moves(self.reversi.valid_moves)
        self.game_state.set_points(Player.Player1, self.reversi.player1_points)
        self.game_state.set_points(Player.Player2, self.reversi.player2_points)
        if self.reversi.is_finished:
            self.game_state.finished()


if __name__ == "__main__":
    app = App()
    app.run()
