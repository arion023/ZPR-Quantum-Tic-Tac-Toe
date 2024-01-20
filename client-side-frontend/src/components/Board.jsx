import { click } from "@testing-library/user-event/dist/click";
import axios from "axios";
import { useEffect } from "react";

function Board({ board, slectedTiles, select, playerSign }) {

    const n = Math.sqrt(board.length);

    function selectTile(idx) {
        select(idx);
    };

    function renderTile(idx) {
        if (slectedTiles.includes(idx)) {
            return board[idx] + playerSign + "_";
        }
        return board[idx];
    };

    function renderRow(rowTiles, row) {
        return (
            <div className="row-container d-flex no-wrap">
                {rowTiles.map((tile, idx) => (
                    <button key={idx} className="tic-tac-toe-tile" onClick={() => selectTile(row * n + idx)}>
                        {renderTile(row * n + idx)}
                    </button>
                ))}
            </div>
        )
    }

    let rows = [];
    for (let i = 0; i <= board.length - n; i = i + n) {
        let row = board.slice(i, i + n);
        rows.push(row);
    }

    return (
        <div className="board-containter">
            {rows.map((row, idx) => renderRow(row, idx))}
        </div>
    );
}

export default Board;