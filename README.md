# 🚕C108 Git Convention!!🚕

## 🚗Sub1🚗

### 🚖 Git-Flow 지켜서 개발할것!

ex) master | develop | hotfix | feature | release

1. Git-Flow 지켜서 개발하기!
2. 모르는건 물어보기!

### 🚖 git flow 전략 사용

- master: 제품으로 출시될 수 있는 브랜치
- develop: 다음 출시 버전을 개발하는 브랜치
- feature: 기능을 개발하는 브랜치 (ex: feature/IMU_jh)
- release: 이번 출시 버전을 준비하는 브랜치
- hotfix: 출시 버전에서 발생한 버그를 수정하는 브랜치

### 🚖 Commit Message

[참고자료](https://chanhuiseok.github.io/posts/git-4/)

- 목적 : 커밋 메시지를 커밋 템플릿을 사용하여 보기 쉽고 간편하게 작성하자.
- 아래 명령어를 입력하고, `git commit` 입력시, 커밋 템플릿이 자동으로 `vi 에디터` 환경으로 나올 것입니다. 3개의 `##########` 부분 윗 줄에 각각 제목, 본문, 꼬릿말을 추가하고 `:wq!`를 입력하고 엔터를 누릅니다.

  ```
  git config --global commit.template <.gitmessage.txt 경로>

  ```

- 커밋 메세지 구성 방법
  ```
  <type>[optional scope]: <description>
  [optional body]
  [optional footer(s)]
  ```
- 제목은 어떤 것을 했는지 명확한 단어가 들어가야 하고, 너무 길게 작성하지 않는다.
- 내용은 선택사항이나 자세한 커밋 메세지를 작성하고자 할 때, 제목에 이어 부가적인 설명을 붙인다. 이 커밋을 한 이유와 변경 내용 등을 작성한다.
- 푸터는 선택사항이고 관련 이슈 번호를 참조시킬 때 주로 사용한다.
