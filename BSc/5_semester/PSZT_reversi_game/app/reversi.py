import copy


class Reversi:
    __directions = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]

    def __init__(self) -> None:
        self.board_size = 8
        self.board = [[0] * self.board_size for x in range(self.board_size)]
        self.__init_board()
        self.current_player = 1
        self.valid_moves = []
        self.valid_moves_list = []
        self.num_of_valid_moves = 0
        self.__update_valid_moves()
        self.is_finished = False
        self.player1_points = self.__count_points(1)
        self.player2_points = self.__count_points(2)

    def __init_board(self) -> None:
        self.board[3][3] = 2
        self.board[3][4] = 1
        self.board[4][3] = 1
        self.board[4][4] = 2

    def reset(self):
        self.__init__()

    def print_board(self) -> None:
        for row in self.board:
            for col in row:
                print(col, end=' ')
            print()

    def print_valid_moves(self) -> None:
        for row in self.valid_moves:
            for col in row:
                if col == -1:
                    print('-', end=' ')
                else:
                    print(col, end=' ')
            print()

    def make_a_move(self, row: int, col: int) -> bool:
        if not self.is_finished and self.__is_valid_move(row, col):
            self.board[row][col] = self.current_player
            for direction in self.__directions:
                self.__flip_in_dir(row, col, direction[0], direction[1])
            self.player1_points = self.__count_points(1)
            self.player2_points = self.__count_points(2)
            self.__next_turn()
            if self.num_of_valid_moves == 0:
                self.__next_turn()
                if self.num_of_valid_moves == 0:
                    self.is_finished = True
            return True
        return False

    def __next_turn(self) -> None:
        self.current_player = 2 if self.current_player == 1 else 1
        self.__update_valid_moves()

    def __update_valid_moves(self) -> None:
        valid_moves = copy.deepcopy(self.board)
        valid_list = []
        count = 0
        for row in range(self.board_size):
            for col in range(self.board_size):
                if self.__is_valid_move(row, col):
                    valid_moves[row][col] = -1
                    valid_list.append((row, col))
                    count += 1
        self.valid_moves = valid_moves
        self.valid_moves_list = valid_list
        self.num_of_valid_moves = count

    def __is_valid_move(self, row: int, col: int) -> bool:
        if self.board[row][col] == 0:
            for direction in self.__directions:
                if self.__is_valid_in_dir(row, col, direction[0], direction[1]):
                    return True
        return False

    def __is_valid_in_dir(self, row: int, col: int, row_dir: int, col_dir: int) -> bool:
        opponent = 2 if self.current_player == 1 else 1
        next_row = row + row_dir
        next_col = col + col_dir
        if not self.__is_out_of_bounds(next_row, next_col) and self.board[next_row][next_col] == opponent:
            while not self.__is_out_of_bounds(next_row, next_col) and self.board[next_row][next_col] != 0:
                if self.board[next_row][next_col] == self.current_player:
                    return True
                next_row += row_dir
                next_col += col_dir
        return False

    def __flip_in_dir(self, row: int, col: int, row_dir: int, col_dir: int) -> None:
        opponent = 2 if self.current_player == 1 else 1
        next_row = row + row_dir
        next_col = col + col_dir
        if self.__is_valid_in_dir(row, col, row_dir, col_dir):
            while not self.__is_out_of_bounds(next_row, next_col) and self.board[next_row][next_col] == opponent:
                self.board[next_row][next_col] = self.current_player
                next_row += row_dir
                next_col += col_dir

    def __is_out_of_bounds(self, row: int, col: int) -> bool:
        if row < 0 or row >= self.board_size or col < 0 or col >= self.board_size:
            return True
        return False

    def __count_points(self, player: int) -> int:
        count = 0
        for row in self.board:
            for col in row:
                if col == player:
                    count += 1
        return count
