# Sokoban
WindowAPI로 제작한 Sokoban 퍼즐 게임

## 게임 포트폴리오 영상

<a href="https://youtu.be/QHLWlav-a8M">
    <img src="https://img.youtube.com/vi/QHLWlav-a8M/0.jpg" width="100%"/>
</a>
이미지를 클릭하면 재생됩니다.

<h2 id="Outline">게임 개요</h2>
<ul>
    <li>
        <h3>Game Genre</h3>
        2D 퍼즐 게임
    </li> 
    <li>
        <h3>Development Stack</h3>
        <p>개발 도구 : <img src="https://custom-icon-badges.demolab.com/badge/Visual%20Studio-5C2D91.svg?&logo=visual-studio&logoColor=white"/>  <img src="https://custom-icon-badges.demolab.com/badge/Window API-003399?style=flat&logo=windows11&logoColor=white"/></p>
        <p>개발 언어 : <img src="https://img.shields.io/badge/C-A8B9CC?style=flat&logo=C&logoColor=white"/> <img src="https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white"/> </p>
    </li>
    <li>
        <h3>Development Period</h3>
        <p>시작 일자 : 2022.11.24</p>
        <p>총 개발 기간 : 3일</p>
    </li>
    <li>
        <h3>Development Member</h3>
        서정현
    </li>
    <li>
        <h3>Target Platform</h3>
        <img src="https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white"/></p>
    </li>
</ul>

## 주요 기능

<ul>
    <li>
        <h3>Game Rule</h3>
        모든 빨간 박스를 초록색 별까지 밀어서 맞추면 승리
    </li> 
    <li>
        <h3>Function key</h3>
        위로 이동 : ↑, 아래로 이동 : ↓, 왼쪽으로 이동 : ←, 오른쪽으로 이동 : →
    </li>
    <li>
        <h3>Implement</h3>
        
1. 6개 물체를 해당 위치로 옮겼을 때 승리판정
2. 게임 진행시간, 성공한 칸 수, 어떤 키를 눌렸는 지(오른쪽 아래 화살표) 표시하는 기능
3. 입력 받은 키를 map이 결과를 처리하여 함수 진행 통제
4. 플레이어와 벽, 벽과 물체, 물체와 물체, 플레이어와 물체 등 충돌 처리
5. 플레이어가 물체와 충돌했을 때 물체가 뒤로 옮겨지는 기
6. 맵 색깔 입히기
    </li>
</ul>
