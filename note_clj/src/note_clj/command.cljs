(ns note-clj.command
  (:require [clojure.string :as str]
            [note-clj.framework :refer [signal effect]]
            [note-clj.note :refer [create-todo-note create-paragraph-note create-header-note]]
            [note-clj.dom :refer [hiccup->dom]]))

(def all-commands
  [{:command "/todo"
    :create #(create-todo-note "新 todo" false)}
   {:command "/p"
    :create #(create-paragraph-note "新片段")}
   {:command "/header"
    :create #(create-header-note "新标题")}])

(defn on-input! [event set-commands!]
  (let [command-value (.-value (.-currentTarget event))
        filtered-commands (filterv
                           (fn [cmd]
                             (str/starts-with? (:command cmd) command-value))
                           all-commands)]
    (if (= command-value "")
      (set-commands! [])
      (set-commands! filtered-commands))))

(defn on-keydown!
  [event add-note! commands set-commands! selected-index set-selected-index!]
  (when-not (.-isComposing event)
    (let [curr-commands (commands)
          curr-commands-count (count curr-commands)]
      (case (.-key event)
        "Enter"
        (do
          (.preventDefault event)
          (when-let [command (get curr-commands (selected-index))]
            (add-note! ((:create command)))
            (set-commands! [])
            (set-selected-index! 0)
            (set! (.-value (.-currentTarget event)) "")))

        "ArrowDown"
        (when (pos? curr-commands-count)
          (.preventDefault event)
          (set-selected-index! (mod (inc (selected-index)) curr-commands-count)))

        "ArrowUp"
        (when (pos? curr-commands-count)
          (.preventDefault event)
          (set-selected-index! (mod (dec (selected-index)) curr-commands-count)))

        nil))))

(defn mount-command! [container add-note!]
  (let [[commands set-commands!] (signal [])
        [selected-index set-selected-index!] (signal 0)
        command-menu (hiccup->dom [:div {:class "command-menu"}])
        root (hiccup->dom
              [:div {:class "note command-box"}
               [:div {:class "block-gutter"}]
               [:input {:class "note-content"
                        :placeholder "输入 / 使用命令"
                        :on-input #(on-input! % set-commands!)
                        :on-keydown #(on-keydown! % add-note! commands set-commands!
                                                  selected-index set-selected-index!)}]])]
    (effect
     (fn []
       (.replaceChildren command-menu)
       (.append command-menu
                (hiccup->dom
                 (into [:div]
                       (map-indexed (fn [index c]
                                      [:div {:class
                                             (str "command-item" (when (= index (selected-index))
                                                                   " command-item-selected"))
                                             :inner-text
                                             (:command c)}]) (commands)))))))
    (.append root command-menu)
    (.append container root)))
