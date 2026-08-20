(ns note-clj.store
  (:require [note-clj.note :refer [create-header-note create-paragraph-note create-todo-note]]))

(defn note->data [note]
  (let [info (:note-info note)
        [content _] (:content note)
        data {:note-info (select-keys info [:id :type :children :parent])
              :content (content)}]
    (if (= (:type info) "todo")
      (let [[checked _] (:checked note)]
        (assoc data :checked (checked)))
      data)))

(defn data->note [data]
  (let [note-info (:note-info data)]
    (case (:type note-info)
      "header"
      (create-header-note note-info (:content data))
      "paragraph"
      (create-paragraph-note note-info (:content data))
      "todo"
      (create-todo-note note-info (:content data) (:checked data)) 
      nil)))

(defn store-notes! [notes]
  (.setItem js/localStorage "notes" (->> notes
                                         (mapv note->data)
                                         clj->js
                                         (.stringify js/JSON))))

(defn load-notes []
  (if-let [data (.getItem js/localStorage "notes")]
    (mapv data->note (.parse js/JSON data))
    nil))
