# Unreal-project
언리얼엔진4로 개발한 3D 액션 MMORPG

# 조작법
- 이동 : WASD
- 공격 : 좌클릭
- 콤보공격 : 좌클릭 연타
- 찌르기 : 우클릭
- 대쉬공격 : 달리는 중 좌클릭 or 구르기 중 좌클릭
- 달리기 : Shift
- 구르기 : Spacebar
- 점프 : Shift + Spacebar
- 걷기 : ctrl
- 락온 타게팅 : 마우스 휠 버튼
- 락온 타게팅 오른쪽에 위치한 타겟으로 전환 : 마우스 휠 위로 회전
- 락온 타게팅 왼쪽에 위치한 타겟으로 전환 : 마우스 휠 아래로 회전


# 특징
# 네트워크 래그돌 동기화 시스템
![마영전 래그돌 위치 동기화 안됨](https://user-images.githubusercontent.com/12960463/117229348-d2fa3c00-ae55-11eb-8c94-daab355dfa7a.gif)

마비노기 영웅전에서는 래그돌 위치 동기화가 정확히 이루어지지 않아 위와같이 래그돌 상태에서 부활을 통해 캐릭터가 살아날 시 서버의 위치로 클라이언트의 위치가 조정되는 움직임을 보입니다.

![래그돌 위치 동기화 1(sm)](https://user-images.githubusercontent.com/12960463/117230797-c88d7180-ae58-11eb-8803-ff39f4a5fec0.gif)

저는 이러한 현상을 막기위해 네트워크 래그돌 위치 동기화 시스템을 개발했습니다.
화면에 보이는 두 플레이어 모두 클라이언트입니다.
지뢰를 밟고 에어본되어 바닥에 닿는 순간 래그돌로 전환되어 구르게 됩니다.
spacebar를 누르면 래그돌 상태에서 일어서며 래그돌이 해제되는데 이 때 래그돌이 이미 서버의 래그돌 위치와 동기화 되어 있기 때문에 위치 조정이 일어나지 않습니다.

![래그돌 위치 동기화 2(sm)](https://user-images.githubusercontent.com/12960463/117230801-cb886200-ae58-11eb-8a8d-578d5977c9f0.gif)

먼 거리를 날아가 래그돌 상태에서 지면을 꽤 먼거리를 굴러서 서버와 클라이언트간 래그돌 위치의 차이가 꽤 벌어질 만 하지만 일어설 때 조금의 조정의 움직임도 보이지 않습니다.


위의 래그돌 동기화 시스템은 각 클라이언트가 자기 자신이 소유한 액터의 래그돌 연산만 전담하는 방식으로 구현되었습니다.
step 0 :
캐릭터 무브먼트 리플리케이션 종료
(위치 조정에 의한 버벅거림 방지)
step 1 :
액터 소유 클라이언트에서 래그돌 연산 수행
step 2 :
액터 소유 클라이언트에서 래그돌의 위치를 RPC를 통해 서버로 전달
step 3 :
서버는 해당 위치로 액터를 옮기고 위치 변수를 리플리케이트
step 4 :
타 클라이언트에서는 피직스 핸들로 리모트 플레이어의 액터를 서버에서 리플리케이트된 타겟 래그돌 위치로 옮김
step 5 :
타겟 위치가 리플리케이트 되는 주기가 클라이언트의 틱 주기보다 느리기 때문에 다음 리플리케이트 까지 액터의 위치를 보간이동


위는 플레이어 캐릭터의 경우이고
AI 캐릭터는 서버 소유의 액터이기 때문에 래그돌 전환 시 가장 가까이 있는 플레이어를 찾아 해당 플레이어를 소유한 클라이언트에서 래그돌 연산을 수행하게됩니다.

이 시스템의 장점
- 래그돌 애니메이션과 특유의 불규칙한 움직임을 거의 온전히 유지하면서 위치를 거의 완벽하게 동기화 시켜줌
- 데디케이티드 서버에서는 피직스 연산을 일절 수행하지 않음
- 벡터 변수 하나로 래그돌의 위치를 동기화

이 시스템의 문제점
- 피직스 연산중인 클라이언트가 도중에 나가게되면 피직스 연산을 대신 이어나갈 클라이언트를 찾아야 함 ( 아직 미구현 )
- 피직스 연산을 수행해야할 클라이언트가 피직스 연산의 대상이되는 액터를 한 번도 보지 못했다면 피직스 연산 버그 발생 ( 피직스 연산을 온전히 수행 가능한 클라이언트만 선별 검색 하도록 구현할 예정 )

# 그 외 특징
# 발 IK
![발IK](https://user-images.githubusercontent.com/12960463/117233132-7d299200-ae5d-11eb-8fdf-ce9a459c60a6.gif)
PowerIK 플러그인 사용

# 디렉셔널 로코모션 & 기울이기 & 블렌딩
![디렉셔널 로코모션 블렌딩](https://user-images.githubusercontent.com/12960463/117234762-908a2c80-ae60-11eb-87a2-aad9e5d3ae29.gif)

엉덩이의 방향이 매우 자유분방한 모션을 자연스럽게 블렌딩하는 것은 꽤 힘든 작업이었음
기울이는 모션도 더 자연스러운 모션을 위해 애니메이션을 블렌딩 한 결과
-180도 와 180도 에서의 블렌딩을 자연스럽게 하기 위해 백워드 디렉션을 함께 구해서 사용
-90~-180, 90~180도 구간에서는 백워드 디렉션을 구해 -90~90도로 변환한 뒤 린 계산

![디렉셔널 로코모션 블렌딩2](https://user-images.githubusercontent.com/12960463/117235046-0b534780-ae61-11eb-93de-7e2f9104c2bb.gif)

걷기도 자연스럽게
스피드에 따라 lean 수치 조정

![디렉셔널 로코모션 블렌딩3(린)](https://user-images.githubusercontent.com/12960463/117235254-59684b00-ae61-11eb-85f1-f7707509fbd7.gif)

착지모션과 로코모션을 블렌딩 해서 착지의 불안정한 모션을 구현하면서, 걸리적거리지 않는 시원시원한 조작감 구현
