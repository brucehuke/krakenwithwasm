<br>
<b>1. compile brige<br></b>
npm install<br>
npm run build:bridge:linux<br>
<br>
<b>2.comple example flutter project<br></b>
setup flutter2.8.1<br>
flutter config --enable-linux-desktop<br>
cd kraken/example/<br>
flutter run -d linux<br>
<br>
<b>3. release.wasm source is in assemblyscript-demo<br></b>
cd assemblyscript-demo<br>
npm install<br>
npm run asbuild<br>

