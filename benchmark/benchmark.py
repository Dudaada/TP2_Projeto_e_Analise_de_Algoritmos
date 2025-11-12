import sys
import matplotlib.pyplot as plt

def plotar_desempenho(arquivo):
    dimensoes = []
    tempos = []

    try:
        with open(arquivo, 'r') as f:
            linhas = f.readlines()
    except FileNotFoundError:
        print(f"Erro: o arquivo '{arquivo}' não foi encontrado")
        return

    for linha in linhas[1:]:
        if linha.strip() == "":
            continue
        partes = linha.split()
        dimensao = partes[0]
        largura, altura = map(int, dimensao.split('x'))
        tamanho = largura
        tempo = float(partes[1])
        dimensoes.append(tamanho)
        tempos.append(tempo)

    plt.figure(figsize=(8,5))
    plt.plot(dimensoes, tempos, marker='o', linestyle='-', color='blue')
    plt.xlabel('Tamanho (largura)')
    plt.ylabel('Tempo de Execução (segundos)')
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python grafico_desempenho.py <arquivo_de_dados.txt>")
    else:
        nome_arquivo = sys.argv[1]
        plotar_desempenho(nome_arquivo)
