"""
    Usage: python3 program_name [player1_depth] [player2_depth]
    [(y) to compare (no) pruning times for p1] [(y) to show output]")
"""

import time
import matplotlib.pyplot as plt

from app.reversi import Reversi
from app.minimax import Minimax
from app import minimax_nopruning as nopruning

import sys
import os

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python3 program_name [player1_depth] [player2_depth]"
              " [(y) to compare (no) pruning times for p1] [(y) to show output]")
        sys.exit()

    game = Reversi()
    minimax = Minimax()

    p1_points = []
    p2_points = []
    nopruning_times = []
    minimax_times = []
    p1_depth = int(sys.argv[1])
    p2_depth = int(sys.argv[2])
    compare_times = sys.argv[3] == "y"
    show_output = sys.argv[4] == "y"

    while not game.is_finished:
        move = (-1, -1)
        move_np = (-1, -1)

        if game.current_player == 1:
            timestamp = time.process_time()
            move = minimax.get_optimal_move(game, p1_depth)
            minimax_times.append(time.process_time() - timestamp)
            move_np = move
            if compare_times:
                timestamp = time.process_time()
                move_np = nopruning.get_optimal_move(game, p1_depth)
                nopruning_times.append(time.process_time() - timestamp)
        else:
            timestamp = time.process_time()
            move = minimax.get_optimal_move(game, p2_depth)
            move_np = move

        assert move == move_np
        assert game.make_a_move(move[0], move[1])
        p1_points.append(game.player1_points)
        p2_points.append(game.player2_points)

        if show_output:
            print("")
            print(f"Player: {game.current_player}")
            print(f"Time: {time.process_time() - timestamp}")
            game.print_board()
            print(f"P1 -- P2 points: {game.player1_points}  --  {game.player2_points}")

    # Display plot showing points over time
    plt.ylabel("Points")
    plt.xlabel("Move number")
    plt.plot(p1_points, color="black")
    plt.plot(p2_points, color="red")
    plt.legend([f"P1 (depth = {p1_depth})", f"P2 (depth = {p2_depth})"])
    if not os.path.exists("plots"):
        os.mkdir("plots")
    plt.savefig(f"plots/points_{p1_depth}_vs_{p2_depth}.png")
    if show_output:
        plt.show()

    if compare_times:
        plt.clf()
        plt.title(f"depth = {p1_depth}")
        plt.ylabel("Time [s]")
        plt.xlabel("Move number")
        plt.plot(nopruning_times, color="black")
        plt.plot(minimax_times, color="red")
        plt.legend(["Minimax only", "With pruning"])
        plt.savefig(f"plots/times_{p1_depth}_vs_{p2_depth}.png")
        if show_output:
            plt.show()
