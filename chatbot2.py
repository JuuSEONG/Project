import streamlit as st
import ollama

st.title("라마3 챗봇 테스트")

if "messages" not in st.session_state:
    st.session_state["messages"] = [{"role": "assistant", "content": "당신의 코딩 실력을 1, 2, 3중에 골라주세요. 숫자가 클수록 숙련된 프로그래머입니다!" }]    #Streamlit 세션 상태에 대화의 첫번째 메세지  assistant = 챗봇, user = 사용자

for msg in st.session_state.messages:   #메세지 세션 상태의 각 메세지에 대해 반복
    if msg["role"] == "user":  #매세지의 역할이 사용자인 경우  
        st.chat_message(msg["role"]).write(msg["content"])  #사용자의 메세지를 채팅 창에 출력, 내용은'msg["content"]
    else:
        st.chat_message(msg["role"]).write(msg["content"])  #챗봇의 메세지를 채팅 창에 출력, 내용은'msg["content"]

def generate_response():
    response = ollama.chat(model='llama3', stream=True, messages=st.session_state.messages)
    for partial_resp in response:   #생성된 응답을 반복처
        token = partial_resp["message"]["content"]  #각 부분 응답 메세지 내용 추출
        st.session_state["full_message"] += token   #각 부분 응답을 전체 메세지에 추가, 전체 대화 히스토리 추적
        yield token     #각 부분 처리된 응답을 생성하여 반환

if prompt := st.chat_input():   #사용자로부터 대화 입력을 처리
    st.session_state.messages.append({"role": "user", "content": prompt})   #사용자의 입력을 대화 히스토리에 추가, 이를 위해 'st.session.state.messages'에 사용자의 역할과 입력 내용을 포함하는 딕셔너리를 추가
    st.chat_message("user").write(prompt)  #사용자의 입력을 채팅창에 출력
    st.session_state["full_message"] = ""   #전체 메세지를 초기화
    st.chat_message("assistant").write_stream(generate_response)    #챗봇의 응답을 생성하고 출력, 이를 위해 'generate_response' 함수를 호출하여 응답을 생성하고, 스트림으로 출력
    st.session_state.messages.append({"role": "assistant", "content": st.session_state["full_message"]})    #챗봇의 응답을 대화 히스토리에 추가, 이를 위해 챗봇의 역할과 응답 내용을 포함하는 딕셔너리 'st.session_state.messages'에 추가
