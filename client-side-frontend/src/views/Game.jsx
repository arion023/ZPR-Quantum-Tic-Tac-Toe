import Board from "../components/Board";
import { useNavigate, useParams } from "react-router-dom";
import axios from "axios";

import "../stylesheets/Game.scss"
import { useState, useEffect } from "react";


function Game(props) {

    console.log("render")

    const navigate = useNavigate();

    const { gameId } = useParams();

    const [response, setResponse] = useState("None");
    const [board, setBoard] = useState([["Error"]]);
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

    //update response
    useEffect(() => {
        console.log("response");
        console.log(response);
        if (response != "None") {
            setBoard(parseBoard(response.data.board));
            setPlayer(response.data.currentPlayer);
            setGameStatus(response.data.status.tag);
            if (response.data.status.tag === "cycle") {
                setCycle(response.data.status.cycle);
            }
        }
    }, [response])


    function parseBoard(boardDict) {
        let newBoard = new Array();
        let tileBoard = new Array();

        for (let i in boardDict) {
            let tile = {};
            if (boardDict[i].entanglements != null) {
                let entanglements = ""
                let ent = boardDict[i].entanglements
                for (let e_id in ent) {
                    entanglements += ent[e_id] + e_id + " ";
                }
                tile.entanglements = entanglements;
                newBoard.push(entanglements)
            }
            else {
                tile.sign = boardDict[i].sign;
                newBoard.push(boardDict[i].sign);
            }
            tileBoard.push(tile)
        }
        return tileBoard;
        //return newBoard;
    }

    function getGame() {
        axios.get('/GetGame/' + gameId)
            .then(function (response) {
                setResponse(response);
            })
            .catch(function (error) {
                // ""
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

    function summaryPopup() {
        let winnerText = "Game winner is"
        if (gameStatus === "finished") {

            if (response.data.status.winner === "-") {
                winnerText = "Game over. Draw."
            }
            
            return (
                <div className="summary-popup">
                    <div className="inner-summary">
                        <p>
                            {winnerText}
                        </p>
                        <button className="btn btn-danger btn-lg" onClick={() => navigate("/")}>Start menu</button>
                    </div>
                </div>
            )
        }
        else
            return ""
    }


    function boardTemplate() {

        if (gameStatus === 'cycle') {
            return (
                <div>
                    <div className="container">
                        <Board board={board} slectedTiles={[measurement]} select={checkAndSetMeasurement} playerSign={flipSign(player)} cycle={cycle} />
                    </div>
                    <div className="container">
                        <button id="make-move" onClick={() => makeMeasurement()} className="btn btn-danger btn-lg">Make measurement</button>
                    </div>
                </div>
            )
        } else {
            return (
                <div>
                    <div className="container">
                        <Board board={board} slectedTiles={move} select={pushToMove} playerSign={player} />
                    </div>
                    <div className="container">
                        <button id="make-move" onClick={() => makeMove()} className="btn btn-warning btn-lg">Make move</button>
                    </div>
                </div>
            )
        }

    }


    return (
        <div className='App container-fluid'>
            <div className="row">
                <div className="col-md-2 fixed-width-sidebar overlay">
                    <div>
                        <h4>Game Info</h4>
                        <p>
                            <span className="badge bg-primary">Game ID:</span> {gameId}
                        </p>
                        <p>
                            <span className="badge bg-success">Status:</span> {gameStatus}
                        </p>
                    </div>
                </div>
                <div className="col flex-column">
                    <h1 className="text-info">Quantum Tic-Tac-Toe</h1>
                    <h4 className="text-warning">Turn: {player}</h4>
                    <div>
                        {boardTemplate()}
                    </div>
                    <div>
                        {summaryPopup()}
                    </div>
                </div>
            </div>
        </div>
    );
}


export default Game;
