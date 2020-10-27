# VoiceerP4
Unity向けのEditor拡張であるVoiccerがいいなと思ってUE4向けのPluginを作成してみました。<br>
本家に比べてまだまだ機能不足ですが、使ってみてもらえればです。<br>
本家Voiccer : https://github.com/negipoyoc/Voiceer

UE4版では現状以下の場合のみ設定したボイスが再生されます。<br>
* C++コードをコンパイルした時
* パッケージ作成した時
* Launchした時

デモ動画 :
https://twitter.com/PaperSloth/status/1320701896496091136?s=20

サンプルに使用させていただいたボイス :[巻乃もなか様](https://twitter.com/monaka_0_0_7)<br>
ボイス配付元 : https://makinomonaka.fanbox.cc/posts/768804

# 動作環境
UE4.25.4

# バージョン
0.1.0 (Experimental)<br>
以下の使用上の注意点をよく読んだ上でお使いください<br>
注意点2. に関してはほとんどの方には気にする程の影響がないという認識です<br>

# 使用上の注意点
1. 本Pluginを使用したProjectを起動中に同一VersionのエンジンのProjectを立ち上げることを避けてください<br>
本PluginはEditorを起動中、エンジン内の特定のアセットに変更を行っています。<br>
そのため、別Projectを立ち上げた場合に同一へのアセットの参照を行い挙動が不安定になることがあります<br>

2. UE4 Editorを閉じた時に元の音データに戻していますが、完全ではありません<br>
基本的にはUE4 Editorを閉じた際に元の音データに戻しており、挙動が変わることがない想定ですが<br>
エンジン自体をVCSで管理している場合には元に戻した場合にもNodeの移動によるDiffが発生してします<br>

# 使用方法
1. 本RepositoryをCloneするか、DownloadZIPで取得する
2. 使用したいUE4プロジェクトの直下に「Plugins」フォルダーを作成し、そこにこの「Voiccer4U」以下のファイルを入れる
3. UE4 Editorを起動して「Edit -> Plugins」を開いて一番下にあるVoiccer4UのEnabledにチェックを入れる
![Plugins](https://user-images.githubusercontent.com/8968076/97306777-479a5f80-18a2-11eb-9d80-ff26a3665adb.png)
4. UE4 Editorを再起動する<br>
以上で設定完了です。<br>
これでC++コードをコンパイルした場合やパッケージ化した時などにボイスが再生されるようになります。

# 違うキャラクター等のボイスを使用したい時
1. 「Edit -> Project Settings」でプロジェクト設定を開く
2. 下にスクロールして「Plugins」のカテゴリから「Voiceer4U」を選択
![image](https://user-images.githubusercontent.com/8968076/97309773-ccd34380-18a5-11eb-9d52-242a3a97f8a7.png)
3. 「Root Directory」に使用したいキャラクター名などを入力する<br>
   実際に使用する時にはこの名称のFolderを作成することになります。<br>
また、登録できる音声の最大数を変更したい場合は「Max Resistry Sound」の数を増やせば対応可能です。<br>

# 違うボイスを使用したい時
1. Content Browser内のView Optionsから「Show Plugin Content」にチェックを入れる
![CB](https://user-images.githubusercontent.com/8968076/97307287-d9a26800-18a2-11eb-8502-03d1cbb8490a.png)
2. Voiceer4U Content/MakinoMonaka/以下の下記のフォルダに任意の音声ファイルを入れる<br>
   違うキャラクターのボイスを使用したい時の工程でProject Settingsから「Root Directory」を変更している場合には<br>
   変更した名称のFolderを作成し、既存のFolderと同様のFolder構成になるようにしてください。
   Compile開始時は「OnCompileStart」<br>
   Compile成功時は「OnCompileSuccess」<br>
   Compile失敗時は「OnCompileFailed」<br>
3. 各音声ファイルは連番指定の規定の名前とする<br>
　　例:Compile開始時の場合<br>
  　　「CompileSuccess0」<br>
  　　「CompileSuccess1」<br>
  　　「CompileSuccess2」<br>
      ...<br>
   設定したファイルがランダムで再生されるようになります。<br>
4. 設定後にUE4 Editorを再起動する<br>
   (Pluginの起動時のタイミングで設定しているため、再起動が必要です。ご不便をおかけします)

# エンジンの標準のサウンドに戻したい時
1. 「Edit -> Project Settings」でプロジェクト設定を開く
2. 下にスクロールして「Plugins」のカテゴリから「Voiceer4U」を選択
3. 「Use Engine Default Sound」にチェックを入れてUE4 Editorを再起動<br>
※ EngineをVCSで管理している方はDiffを見ていただければですが<br>
厳密には完全に元通りというわけではないのですが、基本的に挙動は元に戻ります。
