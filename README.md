# _Darkness (SugiEngine)_
![title](https://github.com/JunyaSugita/sugiEngine/assets/94422859/8e9a22ec-88fa-4627-b018-04177bdef3af)

## 概要
このゲームはたくさんの呪文を使いながら敵を倒す一人称アクションゲームです。

---
## 攻撃手段

### 打撃

![attack](https://github.com/JunyaSugita/sugiEngine/assets/94422859/d8ab481c-df1c-4b92-ab8a-ee1cdb7634b8)

    通常攻撃はためる必要なく使用出来ますがダメージが低く敵に近づかないといけないため
    使いどころを見極めましょう。

### 呪文
- ファイアボール

![fireBall](https://github.com/JunyaSugita/sugiEngine/assets/94422859/90dfb47e-a890-48b1-b7ce-6b89b4963671)

        火の弾を打ち出す呪文です。
        着弾時に爆発して当たった敵を燃やし、持続ダメージも与えます。
        まとまっている敵に効果的です。

- マジックミサイル

![magicMissile](https://github.com/JunyaSugita/sugiEngine/assets/94422859/a51cc3db-adbb-4193-8f30-5d64d278d29d)

        攻撃力の低い弾を6発打ち出す呪文です。
        全て当てるとかなりのダメージが出せるほか、弱った複数の敵にとどめを刺すのも効果的です。
 
- アイスボルト

![iceBolt](https://github.com/JunyaSugita/sugiEngine/assets/94422859/e3db4c6b-f8ad-44f1-878c-38cf73146cdf)

        攻撃力が高い氷の弾を打ち出す呪文です。
        当たった敵はスロー状態になり、しばらく動きが遅くなります。
        敵に近づかれたくない時に効果的です。

- チェインライトニング

![lightning](https://github.com/JunyaSugita/sugiEngine/assets/94422859/635cbd84-631f-4291-8941-50a374de69ff)

        伝播する雷を打ち出す呪文です。
        当たった敵の近くの敵2体にも伝播しダメージが入り、短時間痺れさせることが出来ます。
        複数の敵に確実に当てたいときに効果的です。

- エンチャントファイア

![enchantFire](https://github.com/JunyaSugita/sugiEngine/assets/94422859/b382dcc7-2096-441c-a077-138685af1a46)

        自分の杖に一定時間火属性を付与する呪文です。
        打撃攻撃のダメージを上げ、打撃攻撃した敵を燃やし、持続ダメージも与えます。
        敵が多い時に効果的です。
---  
# 技術紹介
### このゲームはDirectX12を使った自作エンジンで制作しています。
### Windows SDK v.10.0.22621.0
![engineLogo](https://github.com/JunyaSugita/sugiEngine/assets/94422859/d9ad3098-9377-4de2-8390-e5587ef10e0f)


- パーティクルエディタ
  
  ![particleEditor](https://github.com/JunyaSugita/sugiEngine/assets/94422859/50b1bb5a-2949-46d5-a92b-4a9ac14cf638)
  
        エディタでパーティクルの細かい情報をリアルタイムで確認しながら設定することが出来ます。
        設定したパーティクルはファイルで出力され、それを読み込んでゲームに利用したり、エディタで再編集したりすることが出来ます。


- ポストエフェクト

![blar](https://github.com/JunyaSugita/sugiEngine/assets/94422859/602fda37-173e-4735-b56b-e5ce83d30f7a)
![cross](https://github.com/JunyaSugita/sugiEngine/assets/94422859/c69bcea9-6150-4092-b21f-9297bdd75658)
![magicMissile](https://github.com/JunyaSugita/sugiEngine/assets/94422859/ec6ec6ec-de6a-4107-8edf-42605d170f69)

        ブラーやクロスフィルタを使用し、見た目をより良くしています。
        これらの設定は画面全体にかける他、各オブジェクトごとに設定することが出来ます。

- レベルエディタ

![stageBle](https://github.com/JunyaSugita/sugiEngine/assets/94422859/4f34bdd8-9f1b-47c3-a784-e3ad0dcbf57e)
![stage](https://github.com/JunyaSugita/sugiEngine/assets/94422859/47eb4bfe-c4ab-40ee-871d-c11933bb02fd)

        pythonで自作のアドオンを作成。
        Blender上でオブジェクトや敵を配置し、それをjsonファイルとして出力。
        それをエンジン上でロードしてゲームに反映させています。
