import enum
from typing import List
from typing import Tuple


class State(enum.Enum):
    """ Enum defining current game state
            BEFORE_START -- before the game started
            START -- game in progress
            FINISHED -- game finished """
    BEFORE_START = 0
    START = 1
    FINISHED = 2
    RESTARTING = 3


class Player(enum.Enum):
    Player1 = 1
    Player2 = 2


class Move:
    """ Defines single game move """

    def __init__(self, row: int, col: int):
        self.row = row
        self.col = col


class Property(enum.Enum):
    IS_HUMAN = 0
    DEPTH = 1
    STARTS_GAME = 2


class GameState:
    """ Class containing current game state
            Is used to communicate between GUI and game engine """

    def __init__(self):
        self.state = State.BEFORE_START
        self.moves = []
        self.board_state = []
        self.valid_moves = []
        self.current_player = Player.Player1
        self.points = {Player.Player1: 0, Player.Player2: 0}
        self.player_properties = {
            Player.Player1: {Property.IS_HUMAN: True, Property.DEPTH: 1, Property.STARTS_GAME: True},
            Player.Player2: {Property.IS_HUMAN: False, Property.DEPTH: 1, Property.STARTS_GAME: False}
        }

    def get_player_property(self, player: Player, player_property: Property):
        return self.player_properties[player][player_property]
    
    def set_player_property(self, player: Player, player_property: Property, value):
        if player_property == Property.STARTS_GAME:
            self.player_properties[player][player_property] = value
            self.player_properties[Player((player.value+1) % 2)][player_property] = not value
        else:
            self.player_properties[player][player_property] = value

    def set_points(self, player: Player, points: int):
        self.points[player] = points

    def get_points(self, player: Player) -> int:
        return self.points[player]

    def get_current_player(self) -> Player:
        return self.current_player
    
    def set_current_player(self, player: Player):
        self.current_player = player

    def get_state(self) -> State:
        """ Return current game state """
        return self.state

    def in_menu(self):
        self.state = State.BEFORE_START

    def restart(self):
        self.state = State.RESTARTING

    def finished(self):
        self.state = State.FINISHED

    def start(self):
        self.state = State.START

    def add_move(self, move: Tuple[int]) -> bool:
        """ Add a new move.
                Move is only added if the game already started.

            Arguments:
                move -- tuple of size 2 with x,y coordinates of the next move

            Return value:
                True -- if the addition was successful
                False -- if the addition was a failure
        """
        if self.state == State.START:
            self.moves.append(Move(move[0], move[1]))
            return True
        return False

    def get_moves(self) -> List[Move]:
        return self.moves

    def clear_moves(self):
        """ Clear the list of moves """
        self.moves.clear()

    def set_board_state(self, state: List[List[int]]):
        self.board_state = state

    def get_board_state(self) -> List[List[int]]:
        return self.board_state

    def set_valid_moves(self, valid_moves: List[List[int]]):
        self.valid_moves = valid_moves

    def get_valid_moves(self) -> List[List[int]]:
        return self.valid_moves
