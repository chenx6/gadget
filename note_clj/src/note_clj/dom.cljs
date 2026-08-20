(ns note-clj.dom
  (:require [note-clj.framework :refer [effect]]))

(def attr-name-map
  {:class "className"
   :for "htmlFor"
   :tabindex "tabIndex"
   :contenteditable "contentEditable"
   :inner-text "innerText"})

(defn hiccup->dom [h]
  (cond
    (nil? h)    (.createTextNode js/document "")
    (string? h) (.createTextNode js/document h)
    (number? h) (.createTextNode js/document (str h))
    (fn? h)
      (let [node (.createTextNode js/document "")]
        (effect
          (fn []
            (let [nv (str (h))]
              (when (not= (.-nodeValue node) nv)
                (set! (.-nodeValue node) nv)))))
        node)
    :else
    (let [[tag & more] h
          has-attrs? (map? (first more))
          attrs (if has-attrs? (first more) nil)
          children (if has-attrs? (rest more) more)
          node (.createElement js/document (name tag))]
      (doseq [[k v] (or attrs {})]
        (let [kname (name k)
              prop (get attr-name-map k kname)]
          (cond
            ;; Binding function to "on-" event handler
            (.startsWith kname "on-")
              (.addEventListener node (.slice kname 3) v)
            ;; Create effect when value is signal getter
            (fn? v)
              (effect
                (fn []
                  (let [nv (v)]
                    (when (not= (get node prop) nv)
                      (aset node prop nv)))))
            :else
              (aset node prop v))))
      (doseq [c children]
        (.append node (hiccup->dom c)))
      node)))
