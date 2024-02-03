import enum
import pygame
from typing import List
import pygame.freetype
from pygame_gui.elements.ui_drop_down_menu import UIDropDownMenu
from pygame_gui.elements.ui_text_entry_line import UITextEntryLine
import time


class Color(enum.Enum):
    NO_COLOR = -1
    BLACK = "#212121"
    WHITE = "#f5f5f5"
    GREEN = "#388e3c"
    ORANGE = "#ffc107"
    BROWN = "#795548"


class State(enum.Enum):
    """ Different states of the board tile
            UNCHECKED -- tile is unchecked (GREEN)
            BLACK -- checked black
            WHITE -- checked white """
    UNCHECKED = 0
    BLACK = 1
    WHITE = 2


class Board:
    """ Represents tiled game board """

    class Tile:
        """ Represents single board tile """
        """
        big_rect is always black it is used to display border around tiles
        if tile is in POSSIBLE_CHECK state:
            * normal_rect is orange
            * small rect is green
        else:
            * normal_rect is green
            * small_rect is not displayed
        """
        def __init__(self, screen, x: int, y: int):
            self.screen = screen
            self.x = x
            self.y = y
            self.big_rect = pygame.Rect(
                self.x - Board.tile_size,
                self.y - Board.tile_size,
                2 * Board.tile_size,
                2 * Board.tile_size
            )
            self.normal_rect = pygame.Rect(
                self.x - Board.tile_size + Board.tile_b_size,
                self.y - Board.tile_size + Board.tile_b_size,
                2 * Board.tile_size - 2 * Board.tile_b_size,
                2 * Board.tile_size - 2 * Board.tile_b_size
            )
            self.small_rect = pygame.Rect(
                self.x - Board.tile_size + Board.tile_b_size2,
                self.y - Board.tile_size + Board.tile_b_size2,
                2 * Board.tile_size - 2 * Board.tile_b_size2,
                2 * Board.tile_size - 2 * Board.tile_b_size2
            )

        def draw(self, state: State, is_possible_check: bool):
            """ Draws a tile """
            pygame.draw.rect(self.screen, Color.BLACK.value, self.big_rect)

            if is_possible_check:
                pygame.draw.rect(self.screen, Color.ORANGE.value, self.normal_rect)
                pygame.draw.rect(self.screen, Color.GREEN.value, self.small_rect)
            else:
                pygame.draw.rect(self.screen, Color.GREEN.value, self.normal_rect)

            if state == State.BLACK:
                pygame.draw.circle(self.screen, Color.BLACK.value, (self.x, self.y),
                                   Board.tile_size - Board.tile_padding)
            elif state == State.WHITE:
                pygame.draw.circle(self.screen, Color.WHITE.value, (self.x, self.y),
                                   Board.tile_size - Board.tile_padding)

        def is_clicked(self, x, y):
            """ Returns True if tile was clicked """
            return self.big_rect.collidepoint((x, y))

    tile_size = 40
    tile_padding = 10
    # outer tile border size
    tile_b_size = 2
    # border size of the orange indicator displayed when there is possible move on the tile
    tile_b_size2 = 6
    rows = 8
    cols = 8

    def __init__(self, screen, board_x=0, board_y=0):
        """ Creates tiled board of size 8x8 """
        self.tiles = [
            [
                Board.Tile(screen, board_x + (2*c + 1)*self.tile_size, board_y + (2*r + 1)*self.tile_size)
                for c in range(self.cols)
            ]
            for r in range(self.rows)
        ]

    def draw(self, board_state, valid_moves):
        y = 0
        for row in self.tiles:
            x = 0
            for tile in row:
                tile.draw(State(board_state[x][y]), valid_moves[x][y] < 0)
                x += 1
            y += 1

    def is_clicked(self, mouse_x, mouse_y) -> (int, int):
        """ Checks if any tile was clicked
                If a tile was clicked returns its coordinates
                Returns (-1, -1) otherwise """
        y = 0
        for row in self.tiles:
            x = 0
            for tile in row:
                if tile.is_clicked(mouse_x, mouse_y):
                    return x, y
                x += 1
            y += 1
        return -1, -1


class Text:
    """ Simple text field """

    def __init__(self, screen, x: int, y: int, text: str, color: Color = Color.BLACK):
        self.font = pygame.freetype.SysFont('Comic Sans MS', 24)
        self.x = x
        self.y = y
        self.screen = screen
        self.text = text
        self.color = color

    def set_text(self, text: str):
        self.text = text

    def draw(self):
        text_surface, rect = self.font.render(self.text, self.color.value)
        self.screen.blit(text_surface, (self.x, self.y))


class DropDownWithCaption:
    """ Dropdown list with caption """

    def __init__(self, screen, ui_manager, x: int, y: int,
                 options_list: List[str], starting_option: str, caption_text: str):
        self.x = x
        self.y = y
        self.screen = screen
        self.caption = Text(screen, x, y, caption_text)
        self.current_option = starting_option
        self.dropdown = UIDropDownMenu(options_list=options_list,
                                       starting_option=starting_option,
                                       relative_rect=pygame.Rect((x, y+24), (140, 40)),
                                       manager=ui_manager)

    def set_caption(self, caption_text: str):
        self.caption.set_text(caption_text)

    def update_current_option(self, option: str):
        self.current_option = option

    def get_current_option(self) -> str:
        return self.current_option

    def draw(self):
        self.caption.draw()


class TextBoxWithCaption:
    """ Text input box with caption """

    def __init__(self, screen, ui_manager, x: int, y: int, caption_text: str, initial_value: str = "1"):
        self.x = x
        self.y = y
        self.screen = screen
        self.caption = Text(screen, x+7, y, caption_text)
        self.text_box = UITextEntryLine(relative_rect=pygame.Rect((x, y+24), (30, 30)),
                                        manager=ui_manager)
        self.text_box.set_text(initial_value)

    def get_int(self) -> int:
        """ Returns the text that is currently in the box. Returns 1 if it is empty """
        text = self.text_box.get_text()
        if text == "":
            return 1
        else:
            return int(self.text_box.get_text())

    def __validate_input(self):
        text = ""
        try:
            text = self.text_box.get_text()
            val = int(text)
            if val > 10:
                self.text_box.set_text("10")
            elif val <= 0:
                self.text_box.set_text("1")
        except ValueError:
            if text == "":
                self.text_box.set_text(text)
            else:
                self.text_box.set_text("1")

    def draw(self):
        self.__validate_input()
        self.caption.draw()


class Timer:
    """ Timer that displays minutes and seconds in mm:ss format """

    def __init__(self, screen, x: int, y: int):
        self.font = pygame.freetype.SysFont('Comic Sans MS', 24)
        self.x = x
        self.y = y
        self.screen = screen
        self.seconds = 0
        self.minutes = 0
        self.started = False

    def tick(self):
        if self.started:
            self.seconds += 1
            if self.seconds == 60:
                self.minutes += 1
                self.seconds = 0

    def reset(self):
        self.minutes = self.seconds = 0
        self.started = False

    def start(self):
        self.started = True

    def draw(self):
        t = (2009, 2, 17, 17, self.minutes, self.seconds, 1, 48, 36)
        t = time.mktime(t)
        text = time.strftime("%M:%S", time.gmtime(t))
        text_surface, rect = self.font.render(text, (0, 0, 0))
        self.screen.blit(text_surface, (self.x, self.y))


""" Utility functions """


def draw_arrow(screen, x: int, y: int, size_x: int, size_y: int, color: Color = Color.BLACK):
    pygame.draw.polygon(screen, color.value,
                        ((x, y + 2*size_y), (x, y + 4*size_y), (x + 4*size_x, y + 4*size_y),
                         (x + 4*size_x, y + 6*size_y), (x + 6*size_x, y + 3*size_y),
                         (x + 4*size_x, y), (x + 4*size_x, y + 2*size_y)))
