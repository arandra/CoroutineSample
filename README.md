# CoroutineSample

Visual Studio 2019 가 필요합니다.


std::async 를 사용하여 동기 함수 호출을 비동기 함수호출로 바꾸고,
co_await 와 std::future 를 사용하여 코루틴을 구현하는 예제입니다.

이 경우 시작 스레드(A)와 동기 함수를 실행하는 스레드(B) 동기 함수가 완료 되었을 때 호출되는 스레드(C)가 모두 다른 스레드가 됩니다.

custom::resumable 을 사용하여 동기 함수가 완료 되었을 때 호출되는 스레드(C)와 시작 스레드(A)가 같도록 구현해 보았습니다.  


실행 예

 --------------------- sample with future ------------------------
 
 A: Start WaitInputNumber in thread id 6172
 
 B: GetInputNumber in thread id 2576
  - Input number and press [enter] : 1
 
 C: Resume WaitInputNumber in thread id 2948
  - Input number is 1. -
 
 D: Finished in thread id 6172

 --------------------- sample with custom resumable ------------------------
 
 A: Start WaitInputNumber in thread id 6172
 
 B: GetInputNumber in thread id 2576
  - Input number and press [enter] : 2
 
 C: Resume WaitInputNumber in thread id 6172
  - Input number is 2. -
 
 D: Finished in thread id 6172
