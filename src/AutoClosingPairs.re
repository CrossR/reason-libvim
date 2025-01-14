open EditorCoreTypes;

module AutoClosingPair = {
  type t = {
    opening: string,
    closing: string,
  };

  let create = (~opening, ~closing, ()) => {opening, closing};
};

type t = {pairs: list(AutoClosingPair.t)};

let empty: t = {pairs: []};

let create: list(AutoClosingPair.t) => t =
  (p: list(AutoClosingPair.t)) => {pairs: p};

let closingPairs = ref(empty);
let enabled = ref(false);

let setEnabled = e => enabled := e;
let getEnabled = () => enabled^;

let setPairs = (acp: t) => {
  closingPairs := acp;
};

let isMatchingPair = c => {
  List.exists((p: AutoClosingPair.t) => p.opening == c, closingPairs^.pairs);
};

let isOpeningPair = c => {
  let currentPairs = closingPairs^;
  List.exists((p: AutoClosingPair.t) => p.opening == c, currentPairs.pairs);
};

let isClosingPair = c => {
  let currentPairs = closingPairs^;
  List.exists((p: AutoClosingPair.t) => p.closing == c, currentPairs.pairs);
};

let getByOpeningPair = c => {
  let currentPairs = closingPairs^;
  let matches =
    List.filter(
      (p: AutoClosingPair.t) => p.opening == c,
      currentPairs.pairs,
    );

  switch (matches) {
  | [hd, ..._] => hd
  | [] => failwith("No matching pair: " ++ c)
  };
};

let isBetweenPairs = (line, index) => {
  let index = Index.toZeroBased(index);
  let len = String.length(line);
  if (index > 0 && index < len) {
    List.exists(
      (p: AutoClosingPair.t) =>
        p.opening == String.sub(line, index - 1, 1)
        && p.closing == String.sub(line, index, 1),
      closingPairs^.pairs,
    );
  } else {
    false;
  };
};

let doesNextCharacterMatch = (line, index, s) => {
  let index = Index.toZeroBased(index);
  let len = String.length(line);
  if (index > 0 && index < len) {
    s == String.sub(line, index, 1);
  } else {
    false;
  };
};
