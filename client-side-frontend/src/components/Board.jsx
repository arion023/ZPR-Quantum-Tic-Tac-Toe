
function Board({ board, slectedTiles, select, playerSign, cycle }) {

    const n = Math.sqrt(board.length);

    function renderTile(idx) {
        let signs = "";
        let selected = "";
        if ("entanglements" in board[idx]) {
            signs = board[idx].entanglements;
        }
        else if (board[idx].sign !== "-") {
            signs = board[idx].sign;
        }

        if (slectedTiles.includes(idx)) {
            signs += " " + playerSign + "_";
            selected = "selected-tile bg-warning"
        }

        if (cycle !== undefined && cycle.includes(idx)) {
            signs = board[idx].entanglements + " " + playerSign;
            return (
                <div className="cycle-tile border border-danger">
                    <div className={selected}>
                        {signs}
                    </div>
                </div>
            )
        }

        return (
            <div className={selected}>
                {signs}
            </div>
        )

    };

    function renderRow(rowTiles, row) {
        return (
            <div className="row-container d-flex no-wrap">
                {rowTiles.map((tile, idx) => (
                    <button key={idx} className="tic-tac-toe-tile" onClick={() => select(row * n + idx)}>
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