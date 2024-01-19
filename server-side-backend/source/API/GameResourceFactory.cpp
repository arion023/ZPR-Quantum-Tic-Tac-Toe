#include "../../headers/API/GameResourceFactory.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>

GameResourceFactory::GameResourceFactory(shared_ptr<GamesContainer> games_container)
    : games_container(games_container) {

    _game_resource = make_shared<Resource>();
    _game_resource->set_path(
        "/get_game/{id: [0-9]*}"
    );

    _game_resource->set_method_handler("GET", [&](const shared_ptr<Session> session){
        get_game_handler(session);
    });
};

shared_ptr<Resource> GameResourceFactory::get_resource() const {
    return _game_resource;
}


int GameResourceFactory::get_game_id(const shared_ptr<Session> session)
{
    const auto& request = session->get_request();
    int id = atoi(request->get_path_parameter("id").c_str());
    return id;
}

void GameResourceFactory::get_game_handler(const shared_ptr<Session> session) {
    cout << "Get game." << endl;
    const int game_id = get_game_id(session);
    shared_ptr<Game> game = games_container->get_game(game_id);

    string response = "{\"Error\": \"Object not found\"}";
    //TODO: sperate this to function (base class)
    if(game != nullptr)
    {
        json response_json =  game->to_json();
        response = response_json.dump();
    }
    cout<<"Response: " <<response << endl;
    session->close(OK, response,
        {{"Content-Length", to_string(response.size())}});

}