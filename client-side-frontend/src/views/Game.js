import Board from "../components/Board";
import { useLocation, useParams } from "react-router-dom";
import axios from "axios";

import "../stylesheets/Game.scss"
import { useState, useEffect } from "react";


function Game(props) {

    console.log("render")

    const location = useLocation();

    const { gameId } = useParams();

    const [response, setResponse] = useState("None");
    const [boardState, setBoard] = useState([["Error"]]);
    const [gameStatus, setGameStatus] = useState("Off");
    const [player, setPlayer] = useState('-');
    const [cycle, setCycle] = useState([]);

    const [move, setMove] = useState([]);
    const [measurement, setMeasurement] = useState([]);


    function pushToMove(value) {
        if (move.length === 0) {
            setMove([value]);
        }
        else if (move.length === 1) {
            if (move.includes(value)) {
                setMove([]);
            }
            else {
                let tmpmove = move.slice();
                tmpmove.push(value);
                setMove(tmpmove);
            }
        } else {
            if (move.includes(value)) {
                setMove([]);
            }
        }
    }

    function checkAndSetMeasurement(value) {
        if (cycle.includes(value)) {
            setMeasurement(value)
        }
    }


    //load game on init
    useEffect(() => {
        console.log("onMount")
        getGame();
    }, [])

    //move validation
    useEffect(() => {
        if (gameStatus == "cycle") {
            console.log("cycle");
            console.log(cycle);
            if (measurement.length > 1) {
                setMove(measurement.slice(-1));
            }
        }
    }, [measurement])

    //update response
    useEffect(() => {
        console.log("response");
        console.log(response);
        if (response != "None") {
            setBoard(parseBoard(response.data.board));
            setPlayer(response.data.currentPlayer);
            setGameStatus(response.data.status);
            if (response.data.status === "cycle") {
                setCycle(response.data.cycle);
            }
        }
    }, [response])


    function parseBoard(boardDict) {
        var newBoard = new Array();
        console.log("boardDict")
        console.log(boardDict)
        for (let i in boardDict) {
            if (boardDict[i].entanglements != null) {

                let signs = ""
                let ent = boardDict[i].entanglements
                for (let e_id in ent) {
                    signs += ent[e_id] + e_id + " ";
                }
                newBoard.push(signs)
            }
            else {
                newBoard.push(boardDict[i].sign);
            }
        }
        return newBoard;
    }

    function getGame() {
        axios.get('/get_game/' + gameId)
            .then(function (response) {
                // handle success
                setResponse(response);
            })
            .catch(function (error) {
                // TODO
                console.log(error);
            })
    }


    function makeMove() {
        if (move.length === 2) {
            postMove(move[0], move[1]);
            setMove([])
        }
        else {
            alert("Invalid move!");
        }
    }

    function makeMeasurement() {
        console.log("cycle")
        console.log(cycle)
        console.log("measurement")
        console.log(measurement)
        if (cycle.includes(measurement)) {
            postMove(measurement, -1);
            setMeasurement(-1);
        }
        else {
            alert("invalid measurement");
        }
    }

    function postMove(tile_idx1, tile_idx2) {
        {
            //TODO: error handling
            axios.post('/games/' + gameId + '/MakeMove', {
                player: player,
                idx1: tile_idx1,
                idx2: tile_idx2
            })
                .then(function (response) {
                    setResponse(response);
                })
                .catch(function (error) {
                    console.log(error);
                });
        }
    }

    function flipSign(sign) {
        if (sign === "X")
            return "O";
        else if (sign == "O")
            return "X";
        else {
            return "-";
        }
    }

    function boardTamplete() {
        if (gameStatus === "ongoing") {
            return (
                <div>
                    <div className="container">
                        <Board board={boardState} slectedTiles={move} select={pushToMove} playerSign={player} />
                    </div>
                    <div className="container">
                        <button id="make-move" onClick={() => makeMove()} className="btn btn-warning btn-lg">Make move</button>
                    </div>
                </div>
            )
        }
        else if (gameStatus == 'cycle') {
            return (
                <div>
                    <div className="container">
                        <Board board={boardState} slectedTiles={[measurement]} select={checkAndSetMeasurement} playerSign={flipSign(player)} />
                    </div>
                    <div className="container">
                        <button id="make-move" onClick={() => makeMeasurement()} className="btn btn-danger btn-lg">Make measurement</button>
                    </div>
                </div>
            )
        }
        else {
            return (
                <div>
                    <p>Error {gameStatus}</p>
                </div>
            )
        }
    }


    return (
        <div className='App Game'>
            <div>
                <h1>Game ID: {gameId} </h1>
                <h2>turn = {player}</h2>
                <h2>status = {gameStatus}</h2>
            </div>
            <div>
                {boardTamplete()}
            </div>

        </div>
    )
}


export default Game;
