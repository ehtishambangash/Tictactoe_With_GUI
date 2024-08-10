#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace std;
using namespace sf;

void HideConsole() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

struct game_assets {
    Image background_i, board_i, reset_i, cross_i, circle_i, blank_i;
    Texture background_t, board_t, reset_t, cross_t, circle_t, blank_t;
    Font font;
};

struct game_sprites {
    Sprite background, board, reset, pieces[9];
    Text text;
};

struct game_state {
    string player_1, player_2, msg;
    int cur;
    bool wait_for_reset;
    char set[9];
    int total_games;
    int games_won_by_player_1;
    int games_won_by_player_2;
    int draw_games;
};

bool load_assets(game_assets& assets, game_sprites& sprites) {
    if (!assets.background_i.loadFromFile("Resources\\background.png") ||
        !assets.background_t.loadFromImage(assets.background_i))
        return false;
    sprites.background.setTexture(assets.background_t);

    if (!assets.board_i.loadFromFile("Resources\\board.png") ||
        !assets.board_t.loadFromImage(assets.board_i))
        return false;
    sprites.board.setTexture(assets.board_t);
    sprites.board.setPosition(Vector2f(0.0f, 100.0f));

    if (!assets.reset_i.loadFromFile("Resources\\reset.png") ||
        !assets.reset_t.loadFromImage(assets.reset_i))
        return false;
    sprites.reset.setTexture(assets.reset_t);
    sprites.reset.setPosition(Vector2f(25.0f, 525.0f));

    if (!assets.circle_i.loadFromFile("Resources\\circle.png") ||
        !assets.circle_t.loadFromImage(assets.circle_i))
        return false;

    if (!assets.cross_i.loadFromFile("Resources\\cross.png") ||
        !assets.cross_t.loadFromImage(assets.cross_i))
        return false;

    if (!assets.blank_i.loadFromFile("Resources\\blank.png") ||
        !assets.blank_t.loadFromImage(assets.blank_i))
        return false;

    if (!assets.font.loadFromFile("Resources\\font.ttf"))
        return false;
    sprites.text.setFont(assets.font);
    sprites.text.setCharacterSize(40);
    sprites.text.setFillColor(Color::Black);
    sprites.text.setPosition(Vector2f(25.0f, 25.0f));

    return true;
}

void load_board(game_sprites& sprites, game_state& state, const Texture& blank_t, int startPlayer) {
    memset(state.set, 0, 9);
    state.wait_for_reset = false;

    Vector2f positions[9] = {
            {10.0f, 110.0f}, {150.0f, 110.0f}, {290.0f, 110.0f},
            {10.0f, 250.0f}, {150.0f, 250.0f}, {290.0f, 250.0f},
            {10.0f, 390.0f}, {150.0f, 390.0f}, {290.0f, 390.0f}
    };
    for (int i = 0; i < 9; i++) {
        sprites.pieces[i].setPosition(positions[i]);
        sprites.pieces[i].setTexture(blank_t);
    }

    state.cur = startPlayer;
    state.msg = (startPlayer == 1) ? "It is " + state.player_1 + "'s turn" : "It is " + state.player_2 + "'s turn";
    sprites.text.setString(state.msg);
}

bool check_win(const game_state& state, int placement) {
    int row = (placement / 3) * 3;
    int col = placement % 3;

    if (state.set[row] == state.cur && state.set[row + 1] == state.cur && state.set[row + 2] == state.cur)
        return true;

    if (state.set[col] == state.cur && state.set[col + 3] == state.cur && state.set[col + 6] == state.cur)
        return true;

    if (state.set[0] == state.cur && state.set[4] == state.cur && state.set[8] == state.cur)
        return true;

    if (state.set[2] == state.cur && state.set[4] == state.cur && state.set[6] == state.cur)
        return true;

    return false;
}

bool check_draw(const game_state& state) {
    for (int i = 0; i < 9; i++) {
        if (state.set[i] == 0) return false;
    }
    return true;
}

void key_press(game_sprites& sprites, game_state& state, const game_assets& assets, Vector2f pos) {
    if (!state.wait_for_reset) {
        for (int i = 0; i < 9; i++) {
            if (sprites.pieces[i].getGlobalBounds().contains(pos)) {
                if (state.set[i] == 0) {
                    sprites.pieces[i].setTexture(state.cur == 1 ? assets.cross_t : assets.circle_t);
                    state.set[i] = state.cur;

                    if (check_win(state, i)) {
                        state.wait_for_reset = true;
                        state.msg = (state.cur == 1) ? state.player_1 + " wins!" : state.player_2 + " wins!";
                        sprites.text.setString(state.msg);

                        
                        state.total_games++;
                        if (state.cur == 1) {
                            state.games_won_by_player_1++;
                        } else {
                            state.games_won_by_player_2++;
                        }

                    } else {
                        if (check_draw(state)) {
                            state.wait_for_reset = true;
                            state.msg = "Draw. Nobody wins.";
                            sprites.text.setString(state.msg);


                            state.total_games++;
                            state.draw_games++;
                        } else {
                            state.cur = (state.cur == 1) ? 2 : 1;
                            state.msg = (state.cur == 1) ? "It is " + state.player_1 + "'s turn" : "It is " + state.player_2 + "'s turn";
                            sprites.text.setString(state.msg);
                        }
                    }
                }
                break;
            }
        }
    }

    if (sprites.reset.getGlobalBounds().contains(pos)) {
        load_board(sprites, state, assets.blank_t, (state.cur == 1) ? 2 : 1);
    }
}

void load_game_stats(game_state& state) {
    ifstream infile("game_stats.txt");
    if (infile.is_open()) {
        infile >> state.total_games >> state.games_won_by_player_1 >> state.games_won_by_player_2 >> state.draw_games;
        infile.close();
    } else {
        state.total_games = 0;
        state.games_won_by_player_1 = 0;
        state.games_won_by_player_2 = 0;
        state.draw_games = 0;
    }
}

void save_game_stats(const game_state& state) {
    ofstream outfile("game_stats.txt");
    if (outfile.is_open()) {
        outfile << "Total Games: " << state.total_games << "\n"
                << "Games Won by " << state.player_1 << ": " << state.games_won_by_player_1 << "\n"
                << "Games Won by " << state.player_2 << ": " << state.games_won_by_player_2 << "\n"
                << "Draw Games: " << state.draw_games;
        outfile.close();
    }
}

int main() {
    game_assets assets;
    game_sprites sprites;
    game_state state;

    cout << "Enter Player 1 name: ";
    getline(cin, state.player_1);
    cout << "Enter Player 2 name: ";
    getline(cin, state.player_2);

    load_game_stats(state);

    if (!load_assets(assets, sprites)) {
        cout << "Unable to load game assets\n";
        system("pause");
        return 0;
    }

    load_board(sprites, state, assets.blank_t, 1);

    HideConsole();

    RenderWindow window(VideoMode(400, 600), "Tic Tac Toe");
    while (window.isOpen()) {
        window.clear();
        window.draw(sprites.background);
        window.draw(sprites.board);
        window.draw(sprites.text);
        window.draw(sprites.reset);

        for (int i = 0; i < 9; i++)
            window.draw(sprites.pieces[i]);

        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Button::Left)
                    key_press(sprites, state, assets, window.mapPixelToCoords(Mouse::getPosition(window)));
            }
        }
    }

    save_game_stats(state);

    return 0;
}