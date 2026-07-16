<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
<head><meta charset="UTF-8"><title>Greeting</title></head>
<body>
    <!-- EL(Expression Language): request attribute를 ${} 로 바로 꺼내 씀 -->
    <h1>${message}</h1>

    <!-- 스크립틀릿(<%= %>): 자바 코드 직접 삽입, 요즘은 지양하지만 개념 학습용 -->
    <p>요청 IP: <%= request.getRemoteAddr() %></p>
    <p>현재 시간: <%= new java.util.Date() %></p>

    <a href="index.jsp">처음으로</a>
</body>
</html>
